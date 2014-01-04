class A(object):
	def __init__(self, name):
		self.name = name
	def add(self, x):
		name = self.name + x
		return A(name)

a = A('dog')
b = a.add('cat')
print b.name