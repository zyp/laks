from SCons.Script import *

def SelectMCU(env, mcu, variant_dir = None):

	spec = env.PlatformSpec(mcu = mcu)

	if len(spec) <= 1:
		print('Unknown MCU: %s' % mcu)
		Exit(1)

	env.SetDefault(
		TOOLCHAIN = '%s-' % spec['toolchains'][0],
	)
	
	env.Replace(
		CC     = '${TOOLCHAIN}gcc',
		CXX    = '${TOOLCHAIN}g++',
		AS     = '${TOOLCHAIN}gcc',
		LINK   = '${TOOLCHAIN}gcc',
		AR     = '${TOOLCHAIN}ar',
		RANLIB = '${TOOLCHAIN}ranlib',
		
		CCFLAGS   = Split('-O2 -Wall -ggdb -ffunction-sections'),
		CXXFLAGS  = Split('-std=c++20 -fno-exceptions -fno-rtti -Wno-pmf-conversions'),
		ASFLAGS   = Split('-c -x assembler-with-cpp'),
		LINKFLAGS = Split('-Wall -nostartfiles -Wl,-T${LINK_SCRIPT} -Wl,--gc-sections'),
		
		CPPPATH = ['${LAKS_PATH}'],
		LIBPATH = ['${LAKS_PATH}/ld_scripts'],
		
		LIB_SOURCES = [],
	)

	env.Replace(
		LINK_SCRIPT = spec.get('ld_script'),
	)

	env.Append(
		CCFLAGS = spec.get('cflags', []),
		LINKFLAGS = spec.get('cflags', []),
		CPPDEFINES = spec.get('define', []),
	)
	
	env.SConscript('${LAKS_PATH}/SConscript', variant_dir = variant_dir, exports = 'env')

def exists():
    return True

def generate(env):
    env.AddMethod(SelectMCU)