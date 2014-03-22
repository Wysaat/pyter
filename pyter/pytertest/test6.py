class D:
   def p(): return 43
   y = 1024
   print(p())
   def h(self):
     x = D.y
     z = D.p()
     return x + z

# d = D()
# print(D.p())
# print(d.h())