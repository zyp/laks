from SCons.Script import *
import jinja2

def jinja2_build(target, source, env):
    template = jinja2.Template(open(str(source[0])).read())

    template_env = env.get('TEMPLATE_ENV', {})

    output = template.render(**template_env) + '\n'

    open(str(target[0]), 'w').write(output)

jinja2_builder = Builder(action = Action(jinja2_build, '$J2COMSTR', varlist = ['TEMPLATE_ENV']))

def Jinja2(env, target, source, **kwargs):
    return env.Jinja2Build(target, source, TEMPLATE_ENV = kwargs)


def exists():
    return True

def generate(env):
    env.Append(
        BUILDERS = {'Jinja2Build': jinja2_builder},
    )

    env.AddMethod(Jinja2)