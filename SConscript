# Don't call this file explicitly, it is called implicitly by the build rules.

Import('env')
Export('env')

env.SConscript('ld_scripts/SConscript')

env.Append(
	LIB_SOURCES = Glob('*.cpp') + Glob('*/*.cpp'),
)

# vim: syn=python
