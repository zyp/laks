import gdb
import gdb.xmethod

class RefWorker(gdb.xmethod.XMethodWorker):
    def __init__(self, result_type):
        self.result_type = result_type

    def get_arg_types(self):
        return None

    def get_result_type(self, obj):
        return self.result_type.reference()
 
    def __call__(self, obj):
        return obj['p'].cast(self.result_type.pointer()).dereference()

class PtrWorker(gdb.xmethod.XMethodWorker):
    def __init__(self, result_type):
        self.result_type = result_type

    def get_arg_types(self):
        return None

    def get_result_type(self, obj):
        return self.result_type.pointer()
 
    def __call__(self, obj):
        return obj['p'].cast(self.result_type.pointer())

class SubscriptWorker(gdb.xmethod.XMethodWorker):
    def __init__(self, result_type):
        self.result_type = result_type

    def get_arg_types(self):
        return [gdb.lookup_type('std::size_t')]

    def get_result_type(self, obj, idx):
        return self.result_type.reference()
 
    def __call__(self, obj, idx):
        return obj['p'].cast(self.result_type.pointer())[idx]

class Method(gdb.xmethod.XMethod):
    def __init__(self, name, worker):
        gdb.xmethod.XMethod.__init__(self, name)
        self.worker = worker
 
class Matcher(gdb.xmethod.XMethodMatcher):
    def __init__(self):
        gdb.xmethod.XMethodMatcher.__init__(self, 'mmio_ptr matcher')

        self.methods = [
            Method('operator*', RefWorker),
            Method('operator->', PtrWorker),
            Method('operator[]', SubscriptWorker),
        ]
 
    def match(self, class_type, method_name):
        if not class_type.name.startswith('mmio_ptr<'):
            return
            
        ptr_type = class_type.template_argument(0)

        for method in self.methods:
            if method.enabled and method.name == method_name:
                return method.worker(ptr_type)

gdb.xmethod.register_xmethod_matcher(None, Matcher(), replace = True)

def get_mmio_ptr_type(pt):
    if pt.name.startswith('mmio_ptr<'):
        return pt.template_argument(0)

    for field in pt.fields():
        if not field.is_base_class:
            continue

        t = get_mmio_ptr_type(field.type)

        if t is not None:
            return t

    return None

class MMIOPtr(gdb.Function):
  """
  Usage is, for instance, p /x *$mmio_ptr(RCC)
  """
  def __init__(self):
    gdb.Function.__init__(self, 'mmio_ptr')

  def invoke(self, obj):
    t = get_mmio_ptr_type(obj.type)
    return obj['p'].cast(t.pointer())

MMIOPtr()

class MMIORef(gdb.Function):
  """
  Usage is, for instance, p /x $mmio_ref(RCC)
  """
  def __init__(self):
    gdb.Function.__init__(self, 'mmio_ref')

  def invoke(self, obj):
    t = get_mmio_ptr_type(obj.type)
    return obj['p'].cast(t.pointer()).dereference()

MMIORef()
