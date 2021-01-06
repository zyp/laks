import os

env = Environment(
    ENV = os.environ,

    toolpath = ['scons_tools'],
    tools = [
        'default',
        'tool_selectmcu',
        'tool_firmware',
    ],
)

Return('env')
