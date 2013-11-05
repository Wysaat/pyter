from __future__ import division

var1 = 7 * 9
var2 = "Hello, Pyter!"
var3 = ["python", "ruby", "erlang", 1139]
var4 = ('hi', 33, 'bye')
var5 = {"monday": 1,
        "tuesday": 2,
       }

inverse_zip = lambda x, y: zip(y, x)
print inverse_zip(var3, var4)

def func1(x, y):
	x = x + 8
	def _func2(x):
		t = x ** 2
		return t
	y = _func2(y - 3)
	return x / y

class Books(dict):
	def add(self, title, price):
		self.update({title: price})

for i in range(3):
	print var1, var2, func1(var1, var1)

i = 10
while i > 4:
	print 'bye'
	i = i - 1