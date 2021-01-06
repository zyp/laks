import os

env = Environment(
    ENV = os.environ,

    toolpath = ['scons_tools'],
    tools = [
        'default',
        'tool_selectmcu',
        'tool_firmware',
        'tool_jinja2',
    ],
)

Return('env')
