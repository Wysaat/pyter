import error

class enviroment(dict):
    pass

env = environment()

env.

class identifier(atom):
    def __init__(self, item):
        self.name = item
    def evaluate(self):
        try:
            return environment[self.name]
        except:
            name_error()

class literal(atom):
    pass

# just implement it... not standard.
class stringliteral(literal):
    def __init__(self, item):
        tp = 'Unicode' if 'u' in item[:2] or 'U' in item[:2] else 'Str'
        if item[0] not in ["'", '"']:
            item = item[1:]
            if item[0] not in ["'", '"']:
                item = item[1:]
        if item[:3] in ['"""', "'''"]:
            item = item[3:-3]
        else:
            item = item[1:-1]
        self.value = Str(item) if _type == 'Str' else Unicode(item)
    def evaluate(self):
        return self.value
    # def add(self, another):
    #     if not any(another.type == _type for _type in ['str', 'unicode',]):
    #         if self.type == 'unicode':
    #             return name_error('type_error: coercing to Unicode: need string or buffer, %s found'
    #                                    % another.type)
    #         elif self.type == 'str':
    #             return name_error('type_error: cannot concatenate str and %s objects'
    #                                    % another.type)
    #     else:
    #         value = self.value + another.value
    #         string = stringliteral()
    #         string.value = value
    #         types = [self.type, another.type]
    #         if 'unicode' in types: string.type = 'unicode'
    #         elif 'str' in types: string.type = 'str'
    #         return string

class integer(literal):
    def __init__(self, item):
        self.value = int(item)
        self.type = "integer"
    def evaluate(self):
        return self.value

class longinteger(literal):
    def __init__(self, item):
        self.value = long(item)
        self.type = "long"
    def evaluate(self):
        return self.value

class floatnumber(literal):
    def __init__(self, item):
        self.value = float(item)
        self.type = "float"
    def evaluate(self):
        return self.value

class imagnumber(literal):
    def __init__(self, item):
        self.value = complex(item)
        self.type = "complex"
    def evaluate(self):
        return self.value

class enclosure(atom):
    pass

class parenth_form(enclosure):
    def __init__(self, expression_list=None):
        self.expression_list = expression_list
        self.type = "tuple"
    def evaluate(self):
        value = []
        if self.expression_list:
            for expression in self.expression_list:
                value.append(expression.evaluate())
        return value

class list_display(enclosure):
    pass

# a list of expressions
class list_expr(list_display):
    def __init__(self, expression_list=None):
        self.expression_list = expression_list
    def evaluate(self):
        value = []
        if self.expression_list:
            for expression in self.expression_list:
                value.append(expression.evaluate())
        return value

# a list comprehension
class list_comp(list_display):
    def __init__(self, expression, comp_for_list, comp_if_list):
        self.expression = expression
        self.comp_for_list = comp_for_list
        self.comp_if_list = comp_if_list
        self.type = 'list'
    def evaluate(self):
        pass

class set_display(enclosure):
    pass

class dictionary_display(enclosure):
    pass

class comp_for(object):
    def __init__(self, target_list, or_test):
        self.target_list = target_list
        self.or_test = or_test

class comp_if(object):
    def __init__(self, expression_nocond):
        self.expression_nocond = expression_nocond

class generator_expression(enclosure):
    def __init__(self, expression, comp_for_list, comp_if_list):
        self.expression = expression
        self.comp_for_list = comp_for_list
        self.comp_if_list = comp_if_list
        self.type = 'generator'
        self.methods = {
            'next': self.next
        }
    def evaluate(self):
        return self
    def call(self, method_name):
        self.methods[method_name]()
    # def evaluate(self):
    #     targets = values = []
    #     for comp in self.comp_for_list:
    #         targets += comp.target_list
    #         targets.reverse()
    #     values = self.comp_for_list[0].evaluate()
    #     for comp in self.comp_for_list[1:]:
    #         values = [[h, t] for h in comp.evaluate() for t in values]
    #     result = []
    #     for value in values:
    #         if all(comp_if.expression_nocond.evaluate() for comp_if in self.comp_if_list):
    #             targets.assign(value)
    #             result.append(self.expression.evaluate())
    #     return result

class yield_expression(enclosure):
    def __init__(self, expression):
        pass

class primary(object):
    def attribute(self, identifier):
        return

class atom(primary):
    pass

class attributeref(primary):
    def __init__(self, primary, identifier):
        self.primary = primary
        self.identifier = identifier
    def evaluate(self):
        pass

class subscription(primary):
    def __init__(self, primary, expression_list):
        self.primary = primary
        self.expression_list = expression_list

class slicing(primary):
    def __init__(self, primary, slice_list):
        self.primary = primary
        self.slice_list = slice_list

class call(primary):
    pass

class power(object):
    def __init__(self, primary, u_expr):
        self.primary = primary
        self.u_expr = u_expr
    def evaluate(self):
        return pow(self.primary, evaluate(), self.u_expr.evaluate())

class u_expr(object):
    def __init__(self, unary, u_expr):
        self.unary = unary
        self.u_expr = u_expr
    def evaluate(self):
        try:
            if self.unary == '-':
                return -self.u_expr.evaluate()
            elif self.unary == '+':
                return sefl.u_expr.evaluate()
            elif self.unary == '~':
                return ~self.u_expr.evaluate()
        except:
            error()

class m_expr(object):
    def __init__(self):
        self.type = 'operator'

class a_expr(object):
    def __init__(self):
        self.type = 'operator'

class shift_expr(object):
    def __init__(self):
        self.type = 'operator'

class and_expr(object):
    def __init__(self):
        self.type = 'operator'

class xor_expr(object):
    def __init__(self):
        self.type = 'operator'

class or_expr(object):
    def __init__(self):
        self.type = 'operator'

class comparison(object):
    def __init__(self):
        self.type = 'comparison'

class not_test(object):
    pass

class and_test(object):
    pass

class or_test(object):
    pass

class conditional_expression(object):
    pass

class expression(object):
    def __init__(self):
        self.type = 'expression'

class expression_nocond(object):
    pass

class lambda_expr(object):
    pass

class lambda_expr_nocond(object):
    pass

class expression_list(object):
    def __init__(self, expressions):
        self.expressions = expressions
        self.type = 'exression_list'
    def append(self, another):
        self.expressions += another.expressions
    def evaluate(self):
        for expression in self.expressions:
            expression.evaluate()