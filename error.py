class not_support_error(object):
    def evaluate(self):
        print 'not_support_error'
        exit()

class base_exception(object):
    pass

class exception(base_exception):
    pass

class name_error(exception):
    def __init__(self, string="name_error"):
        self.string = string
    def evaluate(self):
        print self.string
        exit()

class type_error(exception):
    def __init__(self, string="type_error"):
        self.string = string
    def evaluate(self):
        print self.string
        exit()

class value_error(exception):
    def __init__(self, string="value_error"):
        self.string = string
    def evaluate(self):
        print self.string
        exit()