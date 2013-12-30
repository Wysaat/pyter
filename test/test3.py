class stringliteral(object):
    def __init__(self, item):
        i = 0
        self.type = 'str'
        while item[i] not in ["\"", "\'"]:
            i += 1
        if item[i:i+3] == item[i] * 3:
            self.string = item[:i] + item[i+3:-3]
        else:
            self.string = item[:i] + item[i+1:-1]
        if 'u' in item[:2] or 'U' in item[:2]:
            self.string = self.string.decode('unicode-escape')
            self.type = 'unicode'
        if 'r' in item[:2] or 'R' in item[:2]:
            self.string = self.string.replace('\\', '\\\\')
    def evaluate(self):
        return self.string

a = stringliteral("r'\n?'")
print a.evaluate()