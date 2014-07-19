class A:
	def __gt__(self, x):
		return 1024

class B(A):
	pass

b = B()
print(b.__gt__(3))
print(b > 3)

class C(B):
	pass

c = C()
print(b.__gt__)
print(c.__gt__)
print(B.__gt__)
print(C.__gt__)
