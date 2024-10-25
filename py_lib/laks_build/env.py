import erect
from erect.lib import *

from .platform_spec import platform_spec

import pathlib
import os

laks_path = pathlib.Path(__file__).parent.parent.parent.relative_to(pathlib.Path('.').absolute())

blueprints = [erect.load_blueprint(laks_path / dir / 'build.py') for dir in [
    'startup',
    'periph',
    'legacy_sources',
]]

def do_patch(target, source):
    for k, v in source.items():
        if k in target and isinstance(v, dict):
            do_patch(target[k], v)

        else:
            target[k] = v

class Env(erect.Env):
    def __init__(self, **kwargs):
        super().__init__(cxx_modules = True, **kwargs)

        self.laks_path = laks_path
        self.laks_sources = []
        self.generated_sources = []

    def executable(self, output_file, source_files):
        ld_script = output_file.replace('.elf', '.ld')
        ld_script_task = Jinja2(self, ld_script, self.laks_path / 'ld_scripts/generated.ld.j2', mem = self.platform_spec['mem'])

        return super().executable(output_file,
            [
                *source_files,
                *self.laks_sources,
                *(t.target for t in self.generated_sources),
            ],
            ld_script = ld_script_task.target,
        )

    def select_mcu(self, mcu, *, patch = None):

        self.platform_spec = platform_spec(mcu = mcu)

        assert len(self.platform_spec) > 1, f'Unknown mcu: {mcu}'

        if patch:
            do_patch(self.spec, patch)

        for prefix in self.platform_spec['toolchains']:
            if any((pathlib.Path(path) / f'{prefix}-gcc').exists() for path in os.get_exec_path()):
                self.toolchain_prefix = f'{prefix}-'
                break
        else:
            raise Exception('Couldn\'t find a suitable toolchain.')

        self.cflags = [
            '-O2',
            '-flto',
            '-Wall',
            '-ggdb',
            '-ffunction-sections',
            '-fdata-sections',

            *self.platform_spec.get('cflags', []),
        ]

        self.cxxflags = [
            *self.cflags,

            '-std=c++20',
            '-fno-exceptions',
            '-fno-rtti',
            '-fcoroutines',
            '-Wno-volatile',
        ]

        self.ldflags = [
            '-O2',
            '-flto',
            '-Wall',
            '-ggdb',
            '-ffunction-sections',
            '-fdata-sections',

            '-nostartfiles',
            '-Wl,--gc-sections',

            *self.platform_spec.get('cflags', []),
        ]
        
        self.include_path = [
            self.laks_path / 'compat_headers',
            self.laks_path / 'legacy_headers', # TODO: Remove this once we don't have any legacy headers left.
            self.laks_path, # TODO: Remove this once all headers are in compat_headers or legacy_headers.
        ]

        self.defines = [
            *self.platform_spec.get('define', []),
        ]

        for blueprint in blueprints:
            blueprint.rules(self)
