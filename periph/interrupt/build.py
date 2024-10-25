from laks_build.templates import PeriphInstanceModule

from erect.lib import Jinja2

def rules(env):
    interrupts = env.platform_spec.get('interrupt', {})

    env.generated_sources.append(Jinja2(env, 'interrupt_enums.cpp', env.laks_path / 'periph/interrupt/interrupt_enums.cpp.j2', interrupts = interrupts))

    env.generated_sources.append(Jinja2(env, 'default_handlers.cpp', env.laks_path / 'periph/interrupt/default_handlers.cpp.j2', interrupts = interrupts))

    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.interrupt') as m:
        m.reexports.append('laks.periph.interrupt.enums')

        if 'nvic' in periph:
            env.laks_sources.append(env.laks_path / 'periph/interrupt/nvic.cpp')
            m.imports.append('laks.periph.interrupt.nvic')
            for name, data in periph['nvic'].items():
                m.instances.append({
                    'type': 'NVIC_t',
                    'name': name,
                    'args': [data['offset']],
                })

            m.aliases['SCB'] = 'NVIC.SCB'
            m.aliases['interrupt_ctl'] = 'NVIC'

            env.generated_sources.extend([
                Jinja2(env, 'vectors_nvic.cpp', env.laks_path / 'periph/interrupt/vectors_nvic.cpp.j2', interrupts = interrupts),
            ])

        if 'clic' in periph:
            env.laks_sources.append(env.laks_path / 'periph/interrupt/clic.cpp')
            m.imports.append('laks.periph.interrupt.clic')
            for name, data in periph['clic'].items():
                m.instances.append({
                    'type': 'CLIC_t',
                    'name': name,
                    'args': [data['offset']],
                })

            m.aliases['interrupt_ctl'] = 'CLIC'

            env.laks_sources.append(env.laks_path / 'periph/interrupt/dispatch_clic.cpp')

            env.generated_sources.extend([
                Jinja2(env, 'vectors_clic.cpp', env.laks_path / 'periph/interrupt/vectors_clic.cpp.j2', interrupts = interrupts),
            ])
