# Don't call this file explicitly, it is called implicitly by the build rules.

Import('env')
Export('env')

env.SConscript('ld_scripts/SConscript')

env.Append(
	LIB_SOURCES = [
		env.SConscript('adc/SConscript'),
		env.SConscript('dma/SConscript'),
		env.SConscript('exti/SConscript'),
		env.SConscript('gpio/SConscript'),
		env.SConscript('interrupt/SConscript'),
		env.SConscript('pwr/SConscript'),
		env.SConscript('rcc/SConscript'),
		env.SConscript('rtc/SConscript'),
		env.SConscript('syscfg/SConscript'),
		env.SConscript('timer/SConscript'),
		env.SConscript('uart/SConscript'),
		env.SConscript('i2c/SConscript'),
		env.SConscript('usb/SConscript'),
		env.SConscript('display/SConscript'),
		env.SConscript('wpan/SConscript'),
		Glob('startup/*.cpp'),
		Glob('os/*.cpp'),
	],
)
