def rules(env):
    startup_files = ['entry']
    startup_files.extend(env.platform_spec.get('startup', []))

    for file in startup_files:
        env.laks_sources.append(env.laks_path / (f'startup/{file}.cpp'))
