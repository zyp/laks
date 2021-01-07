from SCons.Script import *

import yaml
import re
import os.path

def load(fn):
    return yaml.load(open(fn), Loader = yaml.FullLoader)

class Matcher:
    pass

class ReMatcher(Matcher):
    def __init__(self, pattern):
        self.pattern = pattern
    
    def match(self, meta, key):
        m = re.match(self.pattern, meta.get(key, ''))

        if m:
            meta.update(m.groupdict())
            return True

        return False

class Import:
    def __init__(self, fn):
        self.fn = fn
        self.data = None
    
    def _load(self):
        if self.data is None:
            self.data = load(self.fn)
    
    def __iter__(self):
        self._load()
        return iter(self.data or [])

yaml.add_constructor('!re', (lambda loader, node: ReMatcher(loader.construct_scalar(node))))

def construct_import(loader, node):
    fn = loader.construct_scalar(node)
    path = os.path.split(loader.stream.name)[0]

    return Import(os.path.join(path, fn))

yaml.add_constructor('!import', construct_import)

def unprefix(s):
    return int(s[:-1]) * 1024**(' kMG'.index(s[-1]))

yaml.add_constructor('!prefixed', (lambda loader, node: unprefix(loader.construct_scalar(node))))
yaml.add_implicit_resolver('!prefixed', re.compile(r'^\d+[kMG]$'))

def match(meta, name, value):
    if isinstance(value, Matcher):
        return value.match(meta, name)

    return meta.get(name) == value

def merge(parent, key, value):
    if key in parent:
        target = parent[key]
        if isinstance(target, dict):
            for k, v in value.items():
                merge(target, k, v)
            return
        
        elif isinstance(target, list):
            target.extend(value)
            return

    parent[key] = value

def parse(state, data):
    for obj in data:
        if not obj:
            continue

        m = obj.get('match')
        if m:
            if not all(match(state['meta'], name, value) for name, value in m.items()):
                continue
        
        for key, value in obj.items():
            if key in ('match', '-'):
                continue

            #state[key] = obj[key]
            merge(state, key, obj[key])
        
        if '-' in obj:
            parse(state, obj['-'])
    
    return state

def PlatformSpec(env, **kwargs):
    state = {
        'meta': kwargs
    }

    fn = str(env.File('${LAKS_PATH}/platforms/index.yaml'))

    data = yaml.load(open(fn), Loader = yaml.FullLoader)

    return parse(state, data)

def exists():
    return True

def generate(env):
    env.AddMethod(PlatformSpec)