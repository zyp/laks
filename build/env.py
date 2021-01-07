import os

env = Environment(
    ENV = os.environ,
    LAKS_PATH = Dir('..'),

    toolpath = ['scons_tools'],
    tools = [
        'default',
        'tool_selectmcu',
        'tool_firmware',
        'tool_jinja2',
        'tool_platform_spec',
    ],
)

Return('env')
