def f(a, b):
	a[-1:] = [a[-1], b]

a = [1,2,3,]
print("a is", a)
f(a, [1,2,])
print("now a is", a)