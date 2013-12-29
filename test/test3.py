class a(object):
	def __init__(self, name):
		self.name = self.rep(name)
	def show(self):
		return self.name
	def rep(self, name):
		return name + name + name

g = a('yo~')
print g.show()