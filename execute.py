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

