class Int(object):
    def __init__(self, value=None):
        if value == None:
            self.value = 0
        else:
            self.value = value
        self.__class__ = 'Int'
    def __add__(self, another):
        pass

class Float(object):
    def __init__(self, value=None):
        if value == None:
            self.value = 0.0

class Str(object):
    def __init__(self, value=None):
        if value == None:
            self.value = ''
        else:
            self.value = value
        self.__class__ = 'Str'
    def __add__(self, another):
        pass

class Unicode(object):
    def __init__(self, value=None):
        if value == None:
            self.value = u''
        else:
            self.value = value
        self.__class__ = 'Unicode'

class Tuple(object):
    def __init__(self):
        self.value = ()
        self.type = 'Tuple'
    def append(item):
        self.value += (item)
    def __add__(self, another):
        if type(another) == 'Tuple':
            tup = Tuple()
            tup.value = self.value + another.value
            return tup
        else:
            return type_error('can only concatenate tuple (not "str") to tuple')

class Generator(object):
    pass