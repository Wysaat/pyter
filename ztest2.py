class A:
	def __mod__(self, x):
		return 3
	def __gt__(self, x):
		return self.x > x

a = A()
print(a % 43)
a.x = 4423
print(a > 999)
a > 333333.323


print(4324 > 4342.22)