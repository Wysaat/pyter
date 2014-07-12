           # b = __builtins__
 # print = b.print
# next = __builtins__.next
                # len = b.len
# range = b.range


def f(x, y):
    return x ** y
a = f(3, 8)
print(a)
print(132423 * 4324);
class pyint:
    def __init__(





            


    	self, item):
    

        self.value = item
        self.type = 'pyint'
    def evaluate(self):
        return self.value
def greet(name):
    print('Hello', name)


greet('Jack')
greet('Jill')
greet('     Bob')
print(3)
for i in range(10):
    print()


a = pyint(greet);

print(a.type)

print(a.evaluate())
print(3242)

a.evaluate()(       


	    "YOU"
	    )

      
  
def f():
        a = 3
        return a * 3
print(f())

print((3+4j)**(4+8j))
print((3+4j)**(4-8j))
print((3-4j)**(4+8j))
print((3-4j)**(4-8j))
print((-3+4j)**(4+8j))
print((-3-4j)**(4+8j))
print((-3+4j)**(4-8j))
print((-3-4j)**(4-8j))
print((-3+4j)**(-4-8j))
print((-3-4j)**(-4+8j))      




class Point:      
  def __init__(self, x, y):
      self.x, self.y = x, y
  def __str__(self):
      return 'Point('+str(self.x)+', '+str(self.y)+')'

a = Point(432,2)
a.__str__()
print(a.__str__())



a = [123, [43,7,9], "hello"]
print(str(a))
print(a)
print(a)
print(str(a))

class G:
    def __abs__(self):
        return 1024234

g = G()
# g.__abs__()
print(abs(g))

class T:
    def __mod__(self, x):
        return self.value % x
    def __init__(self, val):  
        self.value = val ** 2

t = T(12)

# a = t % 4

a = t.__mod__(4)

print(a)



          

          