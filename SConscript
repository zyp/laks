# Don't call this file explicitly, it is called implicitly by the build rules.

Import('env')
Export('env')

env.SConscript('ld_scripts/SConscript')

env.Append(
	LIB_SOURCES = [
		env.SConscript('dma/SConscript'),
		env.SConscript('exti/SConscript'),
		env.SConscript('gpio/SConscript'),
		env.SConscript('interrupt/SConscript'),
		env.SConscript('pwr/SConscript'),
		env.SConscript('rcc/SConscript'),
		env.SConscript('timer/SConscript'),
		env.SConscript('uart/SConscript'),
		env.SConscript('usb/SConscript'),
		env.SConscript('wpan/SConscript'),
		Glob('startup/*.cpp'),
		Glob('i2c/*.cpp'),
		Glob('os/*.cpp'),
	],
)
