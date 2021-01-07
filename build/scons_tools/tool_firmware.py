from SCons.Script import *

def Firmware(env, target, sources, **kwargs):
	objects = [env.Object(f) for f in Flatten([sources, env['LIB_SOURCES']])]
	firmware = env.Program(target, objects, **kwargs)
	env.Depends(firmware, '${LAKS_PATH}/ld_scripts/${LINK_SCRIPT}')
	return firmware

def exists():
    return True

def generate(env):
    env.AddMethod(Firmware)