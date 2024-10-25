from erect.lib import Jinja2

class Template:
    def __init__(self, env, target, source, **kwargs):
        self.env = env
        self.target = target
        self.source = source
        self.data = kwargs

    def _exit(self, task):
        pass

    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        if exc_type is not None:
            return

        self._exit(Jinja2(self.env, self.target, self.source, **self.data))

class PeriphInstanceModule(Template):
    def __init__(self, env, module):
        self.headers = []
        self.imports = []
        self.reexports = []
        self.instances = []
        self.aliases = {}
        self.type_aliases = {}

        super().__init__(env, 
            target = f'{module}.cpp',
            source = env.laks_path / 'templates/periph_instances.cpp.j2',
            module_name = module,
            headers = self.headers,
            imports = self.imports,
            reexports = self.reexports,
            instances = self.instances,
            aliases = self.aliases,
            type_aliases = self.type_aliases,
        )
    
    def _exit(self, task):
        self.env.generated_sources.append(task)
