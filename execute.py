import error

class environment(object):
    def __init__(self):
        self.variables = {}
    def load(self, identifier):
        try:
            return self.variables[identifier]
        except:
            return name_error()
    def store(self, identifier, value):
        self.variables[identifier] = value

env = environment()

class identifier(object):
    def __init__(self, identifier):
        self.identifier = identifier
    def evaluate(self):
        return env.load(self.identifier)

# support a = 1
#         a, b = 1, 2
class assignment(object):
    def __init__(self, target_list, expression_list):
        self.target_list = target_list
        self.expression_list = expression_list
    def evaluate(self):
        target_list = target_list.evaluate()
        expression_list = expression_list.evaluate()
        if len(target_list) != len(expression_list):
            return value_error()
        pairs = zip(target_list, expression_list)
        for pair in pairs:
            env.store(*pair)