environment = {}

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
        i = 0
        self.type = 'str'
        while item[i] not in ["\"", "\'"]:
            i += 1
        if item[i:i+3] == item[i] * 3:
            string = item[:i] + item[i+3:-3]
        else:
            string = item[:i] + item[i+1:-1]
        if 'u' in item[:2] or 'U' in item[:2]:
            string = string.decode('unicode-escape')
            self.type = 'unicode'
        if 'r' in item[:2] or 'R' in item[:2]:
            string = string.replace('\\', '\\\\')
    def evaluate(self):
        return self.string

class integer(literal):
    def __init__(self, item):
        self.value = int(item)
        self.type = 'int'
    def evaluate(self):
        return self.value

class longinteger(literal):
    def __init__(self, item):
        self.value = long(item)
        self.type = 'long'
    def evaluate(self):
        return self.value

class floatnumber(literal):
    def __init__(self, item):
        self.value = float(item)
        self.type = 'float'
    def evaluate(self):
        return self.value

class imagnumber(litearl):
    def __init__(self, item):
        self.value = complex(item)
        self.type = 'complex'
    def evaluate(self):
        return self.value

class enclosure(atom):
    pass

class parenth_form(enclosure):
    pass

class list_display(enclosure):
    pass

class set_display(enclosure):
    pass

class dictionary_display(enclosure):
    pass

class generator_expression(enclosure):
    pass

class yield_expression(enclosure):
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
        try:
            return self.primary.evaluate() ** self.u_expr.evaluate()
        except:
            error()

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
    pass

class a_expr(object):
    pass

class shift_expr(object):
    pass

class and_expr(object):
    pass

class xor_expr(object):
    pass

class or_expr(object):
    pass

class comparison(object):
    pass

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
    pass