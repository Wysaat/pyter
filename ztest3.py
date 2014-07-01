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