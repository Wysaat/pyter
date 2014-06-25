class Point:      
  def __init__(self, x, y):
      self.x, self.y = x, y
  def __str__(self):
      return 'Point('+str(self.x)+', '+str(self.y)+')'

a = Point(432,2)
a.__str__()
print(a.__str__())