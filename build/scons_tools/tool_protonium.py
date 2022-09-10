from re import sub
from SCons.Script import *

import pathlib
import subprocess

def path_translate(env, source):
    protopath = [pathlib.Path(p) for p in env['PROTOPATH']]
    
    for p in protopath:
        f = p / f'{source}.proto'

        if not f.exists():
            continue
        
        return str(p / source)

    return source

def Protonium(env, target_dir, sources):
    return [env.ProtoniumBuild(f'{target_dir}/{source}', path_translate(env, source), PROTOC_OUT_DIR = target_dir) for source in sources]
    #return [env.ProtoniumBuild(f'{target_dir}/{source}', path_translate(env, source)) for source in sources]

def protonium_builder_actions(source, target, env, for_signature):
    #target_dir = target[0].Dir('.')
    target_dir = env['PROTOC_OUT_DIR']

    protoc_path = ' '.join(f'-I{path}' for path in env['PROTOPATH'])

    return f'protoc {protoc_path} $SOURCES --protonium_out={target_dir} --python_out={target_dir} --protonium_python_out={target_dir}'

protonium_builder = Builder(
    generator = protonium_builder_actions,
    #action = 'protoc $SOURCES --protonium_out=$TARGET',
    suffix = '_pb.h',
    src_suffix = '.proto',
)

def exists():
    return subprocess.call('protonium', shell = True, stdout = subprocess.DEVNULL, stderr = subprocess.DEVNULL) == 0

def generate(env):
    if not exists():
        return

    protopath = [arg[2:] for arg in subprocess.check_output('pkg-config protobuf --cflags-only-I', shell = True, encoding = 'utf-8').split() if arg.startswith('-I')]
    protopath += [arg[2:] for arg in subprocess.check_output('protonium --cflags', shell = True, encoding = 'utf-8').split() if arg.startswith('-I')]

    env.Append(
        BUILDERS = {'ProtoniumBuild': protonium_builder},
        PROTOPATH = ['.', *protopath],
    )

    env.ParseConfig('protonium --cflags')

    env.AddMethod(Protonium)
