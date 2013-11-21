from string import letters

digits = '0123456789'

keywords = ['def', 'class',
            'while',
            'for',
            'in',
            'range',
            'if', 'else', 'elif',
            'import', 'from', 'as',]

# # '<>' is deprecated in Python 3
# operators = ['+', '-', '*', '**', '/', '//', '%',
#              '<<', '>>', '&', '|', '^', '~',
#              '<', '>', '<=', '>=', '==', '!=', '<>',]

# # '`' is deprecated in Python 3
# delimiters = ['(', ')', '[', ']', '{', '}', '@',
#               ',', ':', '.', '`', '=', ';',
#               '+=', '-=', '*=', '/=', '//=', '%=',
#               '&=', '|=', '^=', '>>=', '<<=', '**=',]

# def lexical_analyser(string, index):
#     index0 = index
#     while index < len(string):
#         if string[index0:index+1] in keywords:
#             return 'keyword', string[index0:index+1]

op_binary_3 = ['**=',]

op_binary_2 = ['**', '==', '!=', '>=', '<=', '+=', '-=', '*=', '/=', '%=',]

op_binary_1 = ['+', '-', '*', '/',
               '=', '>', '<', '%',
               '|', '&',
               ',',]

op_binary = op_binary_3 + op_binary_2 + op_binary_1

op_unary    = ['~',]

op_others   = ['(', ')', '[', ']', '{', '}',
               ';', ':', '#', '\\',]

str_ops = ['"', "'", "'''", '"""']

operators = op_binary + op_unary + op_others + str_ops

reserved = keywords + operators

class scanner(object):
    def __init__(self):
        self.items = []
        self.line_number = 0

    def get_line(self):
        self.string = raw_input()
        self.index = 0
        self.line_number += 1

    def error(self, string):
        print self.string
        print ' ' * (self.index - 1) + '^'
        print string
        exit()

    def syntax_error(self):
        self.error('syntax_error: invalid syntax')

    def lexical_analyser(self):
        string = self.string
        index = self.index
        index0 = index
        if index < len(string):
            if string[index:].strip() == '':
                item = ''
                index = len(string)
            else:
                while string[index] in [' ', '\t',]:
                    index += 1
                if string[index] in operators:
                    self.read_operator()
                if string[index] in letters:
                    if self.read_string_literal():
                        break
                    self.read_identifier()
                elif string[index] is '_':
                    self.read_identifier()
                elif string[index] in digits + ['.']:
                    self.read_numeric_literal()
                elif string[index] in ['"', "'",]:
                    self.read_string_literal()

    def read_operators(self):
        string = self.string
        index = self.index

    def read_identifier(self):
        string = self.string
        index = self.index
        item = ''
        while string[index] in letters + digits + ['_']:
            item += string[index]
            index += 1
        self.index = index
        self.items.append(item)
        return item

    def read_numeric_literal(self):
        string = self.string
        index = self.index
        item = ''
        is_float = 0
        other_starts = ['0' + digit for digit in digits]
        if string[index] is '.':
            item += string[index]
            index += 1
            is_float = 1
        elif string[index] in digits:
            tmp_ind = index
            while tmp_ind < len(string) and string[tmp_ind] in digits:
                tmp_ind += 1
            if tmp_ind < len(string) and string[tmp_ind] is '.':
                item += string[index:tmp_ind+1]
                index = tmp_ind + 1
                is_float = 1
                if index == len(string):
                    self.index = index
                    self.items.append(item)
                    return item
        elif index+1 < len(string) and  \
              string[index:index+2] in ['0x', '0X', '0o', '0O', '0b', '0B'] + other_starts:
            item += string[index:index+2]
            if item in ['0b', '0B']:
                digits = '01'
            elif item in ['0o', '0O'] + other_starts:
                digits = '01234567'
            elif item in ['0x', '0X']:
                digits = '0123456789abcdefABCDEF'
            index += 2
            if index == len(string):
                self.error('syntax_error: invalid token')
            elif string[index] not in digits:
                self.error('syntax_error: invalid token')
            while string[index] in digits:
                item += string[index]
                index += 1
                if index == len(string):
                    self.index = index
                    self.items.append(item)
                    return item
            if string[index] in ['l', 'L']:
                item += string[index]
                index += 1
            self.index = index
            self.items.append(item)
            return item
        if string[index] in digits:
            while string[index] in digits:
                item += string[index]
                index += 1
                if index == len(string):
                    self.index = index
                    self.items.append(item)
                    return item
            if string[index] in ['l', 'L']:
                if not is_float:
                    item += string[index]
                    index += 1
                self.index = index
                self.items.append(item)
                return item
            elif string[index] not in ['e', 'E']:
                self.index = index
                self.items.append(item)
                return item
            else:
                item += string[index]
                index += 1
                if index == len(string):
                    self.error('syntax_error: invalid token')
                if string[index] not in digits + ['+', '-']:
                    self.error('syntax_error: invalid token')
                if string[index] in ['+', '-']:
                    item += string[index]
                    index += 1
                    if index == len(string):
                        self.error('syntax_error: invalid token')
                    if string[index] not in digits:
                        syntax_error('syntax_error: invalid token')
                while string[index] in digits:
                    item += string[index]
                    index += 1
                    if index == len(string):
                        self.index = index
                        self.items.append(item)
                        return item
                self.index = index
                self.items.append(item)
                return item

    def raw_read(self):
        string = self.string
        index = self.index
        if index < len(string):
            if string[index:].strip() == '':
                item = ''
                # for printing error message, 1 more than true index
                index = len(string)
            else:
                while string[index] in [' ', '\t',]:
                    index += 1
                if index+3 <= len(string) and string[index:index+3] in operators:
                    item = string[index:index+3]
                    index += 3
                elif index+2 <= len(string) and string[index:index+2] in operators:
                    item = string[index:index+2]
                    index += 2
                elif index+1 <= len(string) and string[index] in operators:
                    item = string[index]
                    index += 1
                elif string[index] is '.':
                    if index+1 <= len(string) and string[index+1] in digits:
                        item = self.read_numeric_literal()
                    else:
                        item = string[index]
                        index += 1
                elif string[index] in digits:
                    item = self.read_numeric_literal()
                else:
                    item = ''
                    while index < len(string) and string[index] in letters + digits + ['_']:
                    while index < len(string) and string[index] not in [' ', '\t',] + operators \
                          and string[index:index+2] not in operators:
                        item += string[index]
                        index += 1
            self.items.append(item)
            self.index = index
            print 'item:', item
            return item
        else:
            return ''

    def read(self):
        item = self.raw_read()
        if item == '\\':
            self.items.pop()
            if self.raw_read() != '':
                self.error('syntax_error: unexpected character after line continuation character')
            else:
                print '...',
                self.get_line()
                return self.read()
        return item

    def readm(self):
        item = self.read()
        if item == '':
            self.items.pop()
            print '...',
            self.get_line()
            item = self.readm()
        return item

    def scan_noun(self, item=-1):
        if item == -1:
            item = self.read()
        if item is '':
            self.syntax_error()
        elif item is '[':
            self.scan_square_bracket()
        elif item is '(':
            self.scan_bracket()
        elif item is '{':
            self.scan_brace()
        elif item is op_unary:
            self.scan_noun()
        elif item in keywords + operators:
            self.syntax_error()

    def scan_square_bracket(self):
        comma = 1
        while True:
            item = self.readm()
            if item is ']' and comma == 1:
                return
            else:
                self.scan_noun(item)
            item = self.readm()
            if item is ']':
                return
            elif item in op_binary:
                if item is ',': comma = 1
                else: comma = 0
            else:
                self.syntax_error()

def error(string):
    print string
    exit()

def syntax_error():
    error('syntax_error: invalid syntax')

def test():
    sc = scanner()
    sc.get_line()
    while True:
        item = sc.read()
        if item is '':
            break
        print item
    print sc.items

def test():
    sc = scanner()
    while True:
        print '>>>',
        sc.get_line()
        can_be_empty = 1
        while sc.string.strip() == '' and sc.string != '':
            print '...',
            sc.get_line()
        while True:
            item = sc.read()
            if can_be_empty and item == '':
                break
            can_be_empty = 0
            sc.scan_noun(item)
            item = sc.read()
            if item != '':
                if item not in op_binary:
                    sc.syntax_error()
            else:
                break


if __name__ == '__main__':
    test()