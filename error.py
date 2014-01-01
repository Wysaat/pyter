class base_exception(object):
	pass

class exception(base_exception):
	pass

class name_error(exception):
	def __init__(self, string, traceback):
		self.string = string