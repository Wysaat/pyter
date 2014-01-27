class environment(object):
    def __init__(self):
        self.variables = {}
        self.handlers = []
        ret_flag = 0
    def load(self, identifier):
        return self.variables[identifier]
    def store(self, identifier, value):
        self.variables[identifier] = value

env = environment()

class identifier(object):
    def __init__(self, identifier):
        self.identifier = identifier
        self.type = 'identifier'
    def evaluate(self):
        try:
            return env.variables[self.identifier]
        except KeyError:
            return exception('name_error').evaluate()
    def assign(self, value):
        env.store(self.identifier, value)
    def call(self, argument_list):
        function = env.load(self.identifier)
        print function
        pairs = zip(function.parameter_list, argument_list)
        print pairs
        for pair in pairs:
            env.store(pair[0], pair[1].evaluate())
        for statement in function.suite.statements:
            if isinstance(statement, stmt_list):
                for stmt in statement.statements:
                    retval = stmt.evaluate()
                    if env.ret_flag:
                        env.ret_flag = 0
                        return retval
            else:
                retval = statement.evaluate()
                if env.ret_flag:
                    env.ret_flag = 0
                    return retval

class pystr(object):
    def __init__(self, *items):
        self.type = 'pystr'
        self.value = ''
        for item in items:
            index = 0
            while item[index] not in ['"', "'"]: index += 1
            if item[index:index+3] in ['"""', "'''"]:
                item = item[index+3:-3]
            else: item = item[index+1:-1]
            self.value += item
    def evaluate(self):
        return self.value

class pyint(object):
    def __init__(self, item):
        self.value = int(item)
        self.type = 'pyint'
    def evaluate(self):
        return self.value

class pyfloat(object):
    def __init__(self, item):
        self.value = float(item)
        self.type = 'pyfloat'
    def evaluate(self):
        return self.value

class pylist(object):
    def __init__(self, *expressions):
        self.expressions = list(expressions)
        self.type = 'pylist'
    def evaluate(self):
        return [expression.evaluate() for expression in self.expressions]

class comp_for(object):
    def __init__(self, target_list, or_test):
        self.target_list = target_list
        self.or_test = or_test

class comp_if(object):
    def __init__(self, expression_nocond):
        self.expression_nocond = expression_nocond

class list_comp(object):
    def __init__(self, expression, comp_for_list, comp_if_list):
        self.expression = expression
        self.comp_for_list = comp_for_list
        self.comp_if_list = comp_if_list
    def evaluate(self):
        value = []
        comp_for = self.comp_for_list[0]
        comp_if = self.comp_if_list[0] if self.comp_if_list else None
        target = comp_for.target_list[0]
        or_test = comp_for.or_test.evaluate()
        for val in or_test:
            target.assign(val)
            if not comp_if or comp_if and comp_if.expression_nocond.evaluate():
                value.append(self.expression.evaluate())
        return value

class target_list(object):
    def __init__(self, *expressions):
        self.expressions = list(expressions)

class subscription(object):
    def __init__(self, primary, expression):
        self.primary = primary
        self.expression = expression
        self.type = 'subscription'
    def evaluate(self):
        primary = self.primary.evaluate()
        index = self.expression.evaluate()
        return primary[index]
    def assign(self, val):
        index = self.expression.evaluate()
        value = self.primary.evaluate()
        value[index] = val
        self.primary.assign(value)

class slice_item(object):
    def __init__(self, start, stop, step):
        self.start = start
        self.stop = stop
        self.step = step
        self.type = 'slice_item'
    def evaluate(self):
        start = self.start.evaluate() if self.start else None
        stop = self.stop.evaluate() if self.stop else None
        step = self.step.evaluate() if self.step else None
        return [start, stop, step]

class slicing(object):
    def __init__(self, primary, slice_item):
        self.primary = primary
        self.slice_item = slice_item
        self.type = 'slicing'
    def evaluate(self):
        primary = self.primary.evaluate()
        return primary.__getitem__(slice(*(self.slice_item.evaluate())))
    def assign(self, val):
        value = self.primary.evaluate()
        value.__setitem__(slice(*(self.slice_item.evaluate())), val)
        self.primary.assign(value)

class call(object):
    def __init__(self, primary, argument_list):
        self.primary = primary
        self.argument_list = argument_list
    def evaluate(self):
        return self.primary.call(self.argument_list)

class power(object):
    def __init__(self, primary, u_expr):
        self.primary = primary
        self.u_expr = u_expr
        self.type = 'power'
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
        self.type = 'u_expr'
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
        self.type = 'b_expr'
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
            if self.op == 'and': return left and right
            if self.op == 'or': return left or right
        except:
            exception('type_error').evaluate()

class comparision(object):
    def __init__(self, comparisions):
        self.comparisions = comparisions
        self.type = 'comparision'
    def evaluate(self):
        for comparision in self.comparisions:
            if not comparision.evaluate():
                return False
        return True

class not_test(object):
    def __init__(self, not_test):
        self.not_test = not_test
        self.type = 'not_test'
    def evaluate(self):
        return not self.not_test.evaluate()

class star_expr_list(object):
    def __init__(self, *star_expr_list):
        self.star_expr_list = list(star_expr_list)
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
            pair[0].assign(pair[1].evaluate())

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
        self.statements = stmt_list
    def evaluate(self):
        return [stmt.evaluate() for stmt in self.statements]

class suite(object):
    def __init__(self, *statements):
        self.statements = list(statements)
    def evaluate(self):
        return [statement.evaluate() for statement in self.statements]

class function(object):
    def __init__(self, identifier, parameter_list, suite):
        self.identifier = identifier
        self.parameter_list = parameter_list
        self.suite = suite
    def evaluate(self):
        env.store(self.identifier, self)

class return_stmt(object):
    def __init__(self, expression_list=None):
        self.expression_list = expression_list.expression_list
    def evaluate(self):
        env.ret_flag = 1
        if self.expression_list:
            return [expression.evaluate() for expression in self.expression_list]
        return

class if_stmt(object):
    def __init__(self, if_expression, if_suite, elif_list, else_suite=None):
        self.if_expression = if_expression
        self.if_suite = if_suite
        self.elif_list = elif_list
        self.else_suite = else_suite
    def evaluate(self):
        if self.if_expression.evaluate():
            return self.if_suite.evaluate()
        elif self.elif_list != []:
            for self.elif_p in self.elif_list:
                if self.elif_p[0].evaluate():
                    return self.elif_p[1].evaluate()
        elif self.else_suite:
            return self.else_suite.evaluate()
        else: return

class while_stmt(object):
    def __init__(self, expression, suite, else_suite=None):
        self.expression = expression
        self.suite = suite
        self.else_suite = else_suite
    def evaluate(self):
        retvals = []
        while self.expression.evaluate():
            retvals.append(self.suite.evaluate())
        if self.else_suite:
            retvals.append(self.else_suite.evaluate())
        return retvals

class exception(object):
    def __init__(self, type):
        self.type = type
    def evaluate(self):
        if not env.handlers:
            print self.type
            exit()
        else:
            return env.handlers[0].evaluate()