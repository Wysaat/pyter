class environment(object):
    def __init__(self):
        self.variables = {}
    def load(self, identifier):
        try:
            return self.variables[identifier]
        except:
            return exception('name_error')
    def store(self, identifier, value):
        self.variables[identifier] = value

env = environment()

class identifier(object):
    def __init__(self, identifier):
        self.identifier = identifier
    def evaluate(self):
        return env.load(self.identifier)
    def store(self, value):
        env.store(self.identifier, value)

class pystr(object):
    def __init__(self, *items):
        self.string = ''
        for item in items:
            index = 0
            while item[index] not in ['"', "'"]: index += 1
            if item[index:index+3] in ['"""', "'''"]:
                item = item[index+3:-3]
            else: item = item[index+1:-1]
            self.string += item
    def evaluate(self):
        return self.string

class pyint(object):
    def __init__(self, item):
        self.value = int(item)
    def evaluate(self):
        return self.value

class pyfloat(object):
    def __init__(self, item):
        self.value = float(item)
    def evaluate(self):
        return self.value

class pylist(object):
    def __init__(self, ):
        pass

class comprehension(object):
    def __init__(self, expression, comp_for_list, comp_if_list):
        pass

class comp_for(object):
    def __init__(self, target_list, or_test):
        self.target_list = target_list
        self.or_test = or_test
    def evaluate(self):
        pass

class target_list(object):
    def __init__(self, *targets):
        self.targets = targets
    def evaluate(self):
        pass

class target(object):

class assignment(object):
    def __init__(self, target_list, expression_list):
        self.target_list = target_list
        self.expression_list = expression_list
    def evaluate(self):
        if len(self.target_list) != len(self.expression_list):
            return exception("value_error")
        pairs = zip(self.target_list, self.expression_list)
        for pair in pairs:
            pair[0].store(pair[1].evaluate())

class exception(object):
    def __init__(self, type):
        self.type = type
    def evaluate(self):
        print self.type
        exit()