from SCons.Script import *
import jinja2

loader = jinja2.FileSystemLoader('.')

jinja2_env = jinja2.Environment(
    loader = loader,
    trim_blocks = True,
    lstrip_blocks = True,
)

jinja2_env.filters['hex'] = lambda value: '%#x' % value
jinja2_env.filters['size_prefix'] = lambda value: '%d%s' % next((value / 1024**i, c) for i, c in [(2, 'M'), (1, 'k'), (0, '')] if value % 1024**i == 0)

def jinja2_build(target, source, env):
    template = jinja2_env.get_template(str(source[0]))

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