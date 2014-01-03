num_types = ['Int', 'Float', 'Long',]

def pow(x, y):
    if type(x) in num_types and type(y) in num_types:
        return __builtins__.pow(x, y)
    else:
        return type_error("unsupported operand type(s) for ** or pow(): %s and %s"
                               % (type(x), type(y)))

def type(object):
    return object.__class__