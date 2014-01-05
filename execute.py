import error

class enviroment(dict):
    pass

env = environment()

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
        item = str2int(item)
        self.value = Int(item)
    def evaluate(self):
        return self.value

class longinteger(literal):
    def __init__(self, item):
        item = str2long(item)
        self.value = Long(item)
    def evaluate(self):
        return self.value

class floatnumber(literal):
    def __init__(self, item):
        item = str2float(item)
        self.value = Float(item)
    def evaluate(self):
        return self.value

class imagnumber(litearl):
    def __init__(self, item):
        item = str2complex(item)
        self.value = Complex(item)
    def evaluate(self):
        return self.value

class enclosure(atom):
    pass

class parenth_form(enclosure):
    def __init__(self, expression_list=None):
        self.expression_list = expression_list
    def evaluate(self):
        tup = Tuple()
        if self.expression_list:
            for expression in self.expression_list:
                tup.append(expression.evaluate())
        return tup

class list_display(enclosure):
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
    def evaluate(self):
        targets = values = []
        for comp in self.comp_for_list:
            targets += comp.target_list
            targets.reverse()
        values = self.comp_for_list[0].evaluate()
        for comp in self.comp_for_list[1:]:
            values = [[h, t] for h in comp.evaluate() for t in values]
        result = []
        for value in values:
            if all(comp_if.expression_nocond.evaluate() for comp_if in self.comp_if_list):
                targets.assign(value)
                result.append(self.expression.evaluate())
        return result

class yield_expression(enclosure):
    def __init__(self, expression):
        pass

class primary(object):
    pass

class atom(primary):
    pass

class attributeref(primary):
    pass

class subscription(primary):
    pass

class slicing(primary):
    pass

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
    pass

class expression_nocond(object):
    pass

class lambda_expr(object):
    pass

class lambda_expr_nocond(object):
    pass

class expression_list(object):
    def __init__(self, expression):
        self.expressions = []
        self.expressions.append(expression)
    def append(self, another):
        self.expressions += another.expressions
    def evaluate(self):
        for expression in self.expressions:
            expression.evaluate()