bin_ops = ['*', '//', '/', '%',
           '+', '-',
           '<<', '>>',
           '&', '^', '|',
           ]

class environment(object):
    def __init__(self):
        self.variables = {}
        self.handlers = []
    def load(self, identifier):
        return self.variables[identifier]
    def store(self, identifier, value):
        self.variables[identifier] = variable(value)

class variable(object):
    def __init__(self, value):
        self.value = value
        self.attributes = {}

env = environment()

class identifier(object):
    def __init__(self, identifier):
        self.identifier = identifier
    def evaluate(self):
        try:
            return env.load(self.identifier).value.evaluate()
        except:
            return exception('name_error').evaluate()
    def assign(self, value):
        env.store(self.identifier, value)
    def load(self):
        try:
            return env.load(self.identifier)
        except:
            return exception('name_error')

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
    def __init__(self, *expressions):
        self.expressions = expressions
    def setitem(self, index, expression):
        try:
            self.expressions[index] = expression
        except:
            return exception('index_error')
    def getitem(self, index):
        return self.expressions[index]
    def evaluate(self):
        return [expression.evaluate() for expression in self.expressions]

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
    def __init__(self, *expressions):
        self.expressions = expressions
    def evaluate(self):
        for expression in expressions:
            if expression.type not in ['attributeref',]:
                return exception('syntax_error')
        targets = self.expressions

class attributeref(object):
    def __init__(self, primary, identifer):
        self.primary = primary
        self.identifier = identifier
    def evaluate(self):
        return self.load().value
    def load(self):
        try:
            primary = self.primary.load()
            return primary.attributes[self.identifier]
        except:
            return exception('name_error')
    def assign(self, expression):
        self.load().value = expression

class subscription(object):
    def __init__(self, primary, expression):
        self.primary = primary
        self.expression = expression
    def assign(self, expression):
        index = self.expression.evaluate()
        primary = self.primary.load()
        try:
            primary.setitem(index, expression)
        except:
            return exception('type_error')

class power(object):
    def __init__(self, primary, u_expr):
        self.primary = primary
        self.u_expr = u_expr
    def evaluate(self):
        u_expr = self.u_expr.evaluate()
        primary = self.primary.evaluate()
        try:
            return primary ** u_expr
        except:
            exception('type_error').evaluate()

class u_expr(object):
    def __init__(self, u_op, u_expr):
        self.u_op = u_op
        self.u_expr = u_expr
    def evaluate(self):
        u_expr = self.u_expr.evaluate()
        try:
            if self.u_op == '-': return -u_expr
            if self.u_op == '+': return u_expr
            if self.u_op == '~': return ~u_expr
        except:
            exception('type_error').evaluate()

class b_expr(object):
    def __init__(self, left, op, right):
        self.left = left
        self.op = op
        self.right = right
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        try:
            if self.op == '*': return left * right
            if self.op == '//': return left // right
            if self.op == '/': return left / right
            if self.op == '%': return left % right
            if self.op == '+': return left + right
            if self.op == '-': return left - right
            if self.op == '<<': return left << right
            if self.op == '>>': return left >> right
            if self.op == '&': return left & right
            if self.op == '^': return left ^ right
            if self.op == '|': return left | right
            if self.op == '<': return left < right
            if self.op == '>': return left > right
            if self.op == '==': return left == right
            if self.op == '>=': return left >= right
            if self.op == '<=': return left <= right
            # if self.op == '<>': return left <> right
            if self.op == '!=': return left != right
            if self.op == 'is': return left is right
            if self.op == 'is not': return left is not right
            if self.op == 'in': return left in right
            if self.op == 'not in': return left not in right
        except:
            exception('type_error').evaluate()

class comparision(object):
    def __init__(self, comparisions):
        self.comparisions = comparisions
    def evaluate(self):
        for comparision in self.comparisions:
            if not comparision.evaluate():
                return False
        return True

class star_expr_list(object):
    def __init__(self, *star_expr_list):
        self.star_expr_list = star_expr_list
    def evaluate(self):
        return [expr.evaluate() for expr in self.star_expr_list]

class expression_list(object):
    def __init__(self, expression_list):
        self.expression_list = expression_list

class assignment(object):
    def __init__(self, star_expr_list, expression_list):
        self.target_list = star_expr_list.star_expr_list
        self.expression_list = expression_list.expression_list
    def evaluate(self):
        if len(self.target_list) != len(self.expression_list):
            return exception("value_error")
        pairs = zip(self.target_list, self.expression_list)
        for pair in pairs:
            pair[0].assign(pair[1])

class try_except(object):
    def __init__(self, suite, handlers):
        self.suite = suite
        self.handlers = handlers
    def evaluate(self):
        env.handlers += self.handlers
        self.suite.evaluate()
        env.handlers = []

class handler(object):
    def __init__(self, suite):
        self.suite = suite
    def evaluate(self):
        return self.suite.evaluate()

class stmt_list(object):
    def __init__(self, stmt_list):
        self.stmt_list = stmt_list
    def evaluate(self):
        return [stmt.evaluate() for stmt in self.stmt_list]

class suite(object):
    def __init__(self, *statements):
        self.statements = statements
    def evaluate(self):
        return [statement.evaluate() for statement in self.statements]

class exception(object):
    def __init__(self, type):
        self.type = type
    def evaluate(self):
        if not env.handlers:
            print self.type
            exit()
        else:
            return env.handlers[0].evaluate()