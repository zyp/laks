def rules(env):
    env.laks_sources.extend(env.laks_path.glob('os/*.cpp'))
