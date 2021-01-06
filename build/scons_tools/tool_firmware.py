from SCons.Script import *

def Firmware(env, target, sources, **kwargs):
	objects = [env.Object(f) for f in Flatten([sources, env['LIB_SOURCES']])]
	firmware = env.Program(target, objects, **kwargs)
	#env.Depends(firmware, env['LINK_SCRIPT']) # TODO
	return firmware

def exists():
    return True

def generate(env):
    env.AddMethod(Firmware)