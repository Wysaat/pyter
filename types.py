class Int(object):
	def __init__(self, value=None):
		if vlaue == None:
			self.value = 0
		else:
			self.value = value
		self.__class__ = 'Int'