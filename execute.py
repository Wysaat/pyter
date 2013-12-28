class identifier(atom):
	pass

class literal(atom):
	pass

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
	def __init__(self, *args):
		if len(args) == 2:
			self.operator, self.u_expr = args
			self.arg_num = 2
		else:
			self.power = args
			self.arg_num = 1
	def evaluate(self):
		if self.arg_num == 2:

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

