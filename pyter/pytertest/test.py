class squares(object):
        def __init__(self, start, stop):
                self.start = start
                self.stop = stop
        def __iter__(self):
                return self
        def next(self):
                if self.start >= self.stop:
                        raise StopIteration
                current = self.start * self.start
                self.start += 1
                return current

class test(object):
        def __getitem__(self, item):
                print type(item), item