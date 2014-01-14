class environment(object):
    def __init__(self):
        self.variables = {}
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

class m_expr(object):
    def __init__(self, left, m_op, right):
        self.left = left
        self.m_op = m_op
        self.right = right
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        try:
            if self.m_op == '*': return left * right
            if self.m_op == '//': return left // right
            if self.m_op == '/': return left / right
            if self.m_op == '%': return left % right
        except:
            exception('type_error').evaluate()

class a_expr(object):
    def __init__(self, left, a_op, right):
        self.left = left
        self.a_op = a_op
        self.right = right
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if self.a_op == '+': return left + right
        if self.a_op == '-': return left - right

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

class exception(object):
    def __init__(self, type):
        self.type = type
    def evaluate(self):
        print self.type
        exit()