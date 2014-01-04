class base_exception(object):
    pass

class exception(base_exception):
    pass

class name_error(exception):
    def __init__(self, string):
        self.string = string
    def evaluate(self):
        print self.string
        exit()

class type_error(exception):
    def __init__(self, string):
        self.string = string
    def evaluate(self):
        print self.string
        exit()