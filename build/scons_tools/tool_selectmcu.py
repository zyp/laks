from SCons.Script import *

def do_patch(target, source):
	for k, v in source.items():
		if k in target and isinstance(v, dict):
			do_patch(target[k], v)
		
		else:
			target[k] = v

def SelectMCU(env, mcu, variant_dir = None, patch = None):

	spec = env.PlatformSpec(mcu = mcu)

	if len(spec) <= 1:
		print('Unknown MCU: %s' % mcu)
		Exit(1)

	if patch:
		do_patch(spec, patch)

	detected_gcc = env.Detect([f'{toolchain}-gcc' for toolchain in spec['toolchains']])
	if detected_gcc is None:
		print('Couldn\'t find a suitable toolchain.')
		print('Toolchains searched for:')
		for toolchain in spec['toolchains']:
			print(f' * {toolchain}')
		Exit(1)

	env.SetDefault(
		TOOLCHAIN = detected_gcc[:-3],
	)
	
	env.Replace(
		PLATFORM_SPEC = spec,

		CC     = '${TOOLCHAIN}gcc',
		CXX    = '${TOOLCHAIN}g++',
		AS     = '${TOOLCHAIN}gcc',
		LINK   = '${TOOLCHAIN}g++',
		AR     = '${TOOLCHAIN}ar',
		RANLIB = '${TOOLCHAIN}ranlib',
		
		CCFLAGS   = Split('-O2 -Wall -ggdb -ffunction-sections -fdata-sections'),
		CXXFLAGS  = Split('-std=c++20 -fno-exceptions -fno-rtti -fcoroutines -Wno-volatile'),
		ASFLAGS   = Split('-c -x assembler-with-cpp'),
		LINKFLAGS = Split('-Wall -nostartfiles -Wl,-T${LINK_SCRIPT} -Wl,--gc-sections'),
		
		LIB_SOURCES = [],
		CPPDEFINES = ['LAKS'],
	)

	env.Replace(
		LINK_SCRIPT = 'generated.ld',
	)

	env.Append(
		CCFLAGS = spec.get('cflags', []),
		LINKFLAGS = spec.get('cflags', []),
		CPPDEFINES = spec.get('define', []),

		CPPPATH = ['${LAKS_PATH}'],
		LIBPATH = ['${LAKS_PATH}/ld_scripts'],
	)
	
	env.SConscript('${LAKS_PATH}/SConscript', variant_dir = variant_dir, exports = 'env')

def exists():
    return True

def generate(env):
    env.AddMethod(SelectMCU)
