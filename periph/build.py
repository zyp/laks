from erect import load_blueprint

def rules(env):
    for blueprint in env.laks_path.glob('periph/*/build.py'):
        load_blueprint(blueprint).rules(env)
