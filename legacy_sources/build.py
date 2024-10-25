def rules(env):
    env.laks_sources.extend(env.laks_path.glob('legacy_sources/*.cpp'))
