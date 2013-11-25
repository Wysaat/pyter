# reference: Python v2.7.2 documentation
from string import letters

digits = '0123456789'

stringprefixes = ['r', 'R', 'u', 'U', 'ur', 'uR', 'Ur', 'UR',
                  'b', 'B', 'br', 'bR', 'Br', 'BR']

keywords = ['and', 'as', 'assert', 'break', 'class', 'continue', 'def',
            'del', 'elif', 'else', 'except', 'exec', 'finally', 'for',
            'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'not',
            'or', 'pass', 'print', 'raise', 'return', 'try','while',
            'with', 'yield']

# '<>' is deprecated in Python 3
operators = ['+', '-', '*', '**', '/', '//', '%',
             '<<', '>>', '&', '|', '^', '~',
             '<', '>', '<=', '>=', '==', '!=', '<>',]

# '`' is deprecated in Python 3
delimiters = ['(', ')', '[', ']', '{', '}', '@',
              ',', ':', '.', '`', '=', ';',
              '+=', '-=', '*=', '/=', '//=', '%=',
              '&=', '|=', '^=', '>>=', '<<=', '**=',]

other_tokens = ['!', '$', '?', '#', '\\',]

tokens = operators + delimiters + other_tokens

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

class scanner(object):
    def __init__(self):
        self.items = []
        self.line_number = 0

    def get_line(self):
        self.string = raw_input()
        self.index = 0
        self.line_number += 1
        return self.string

    def error(self, string):
        print self.string
        print ' ' * (self.index - 1) + '^'
        print string
        exit()

    def syntax_error(self):
        self.error('syntax_error: invalid syntax')

    def read_string_literal(self, item=''):
        string = self.string
        index = self.index
        backslash = 0
        if string[index:index+3] in ['"""', "'''"]:
            item += string[index:index+3]
            op = item[0]
            op_num = 0
            index += 3
            while True:
                if index == len(string):
                    print '...',
                    string = self.get_line()
                    index = 0
                    op_num = 0
                    if backslash == 0:
                        item += '\n'
                else:
                    item += string[index]
                    if string[index] == '\\':
                        if backslash == 0:
                            if index == len(string) - 1:
                                item = item[:-1]
                            backslash = 1
                    elif backslash == 0 and string[index] == op:
                        op_num += 1
                        if op_num == 3:
                            index += 1
                            self.index = index
                            self.items.append(item)
                            return item
                    else:
                        backslash = 0
                    index += 1
        elif string[index] in ['"', "'"]:
            item += string[index]
            op = item
            index += 1
            while True:
                while index < len(string):
                    item += string[index]
                    if backslash == 0 and string[index] == op:
                        index += 1
                        self.index = index
                        self.items.append(item)
                        return item
                    elif backslash == 0 and string[index] == '\\':
                        backslash = 1
                    else:
                        backslash = 0
                    index += 1
                if backslash == 1:
                    item = item[:-1]
                    print '...',
                    string = self.get_line()
                    index = 0
                    backslash = 0
                else:
                    self.index = index
                    self.error('syntax_error: EOL while scanning string literal')

    def read_numeric_literal(self):
        digits = '0123456789'
        string = self.string
        index = self.index
        item = ''
        is_float = 0
        is_imaginary = 0
        other_starts = ['0' + digit for digit in digits]
        if index+1 < len(string) and string[index:index+2] in ['0x', '0X', '0o', '0O', '0b', '0B']:
            item += string[index:index+2]
            if item in ['0b', '0B']:
                digits = '01'
            elif item in ['0o', '0O']:
                digits = '01234567'
            elif item in ['0x', '0X']:
                digits = '0123456789abcdefABCDEF'
            index += 2
            if index == len(string):
                self.index = index
                self.error('syntax_error: invalid token')
            elif string[index] not in digits:
                self.index = index
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
        if index+1 < len(string) and string[index:index+2] in other_starts:
            invalid = 0
            while index < len(string) and string[index] in digits:
                item += string[index]
                if int(string[index]) > 7:
                    invalid = 1
                index += 1
            if invalid:
                if index == len(string):
                    self.index = index
                    self.error('syntax_error: invalid token')
                elif string[index] not in '.eEjJ':
                    self.index = index
                    self.error('syntax_error: invalid token')
            if index == len(string):
                self.index = index
                self.items.append(item)
                return item
        if string[index] is '.':
            item += string[index]
            index += 1
            is_float = 1
            if index == len(string):
                self.index = index
                self.items.append(item)
                return item
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
        while string[index] in digits:
            item += string[index]
            index += 1
            if index == len(string):
                self.index = index
                self.items.append(item)
                return item
        if string[index] in 'lL':
            if not is_float:
                item += string[index]
                index += 1
            self.index = index
            self.items.append(item)
            return item
        elif string[index] in 'jJ':
            item += string[index]
            index += 1
            self.index = index
            self.items.append(item)
            return item
        elif string[index] not in 'eE':
            self.index = index
            self.items.append(item)
            return item
        else:
            item += string[index]
            index += 1
            if index == len(string):
                self.index = index
                self.error('syntax_error: invalid token')
            if string[index] not in digits + '+-':
                self.index = index
                self.error('syntax_error: invalid token')
            if string[index] in ['+', '-']:
                item += string[index]
                index += 1
                if index == len(string):
                    self.index = index
                    self.error('syntax_error: invalid token')
                if string[index] not in digits:
                    self.index = index
                    syntax_error('syntax_error: invalid token')
            while string[index] in digits:
                item += string[index]
                index += 1
                if index == len(string):
                    self.index = index
                    self.items.append(item)
                    return item
            if string[index] in 'jJ':
                item += string[index]
                index += 1
            self.index = index
            self.items.append(item)
            return item

    def raw_read(self):
        string = self.string
        index = self.index
        digits = '0123456789'
        if index < len(string):
            if string[index:].strip() == '':
                item = ''
                # for printing error message, 1 more than true index
                index = len(string)
            else:
                while string[index] in [' ', '\t',]:
                    index += 1
                    self.index = index
                if index+2 < len(string) and string[index:index+3] in tokens:
                    item = string[index:index+3]
                    index += 3
                    self.items.append(item)
                    self.index = index
                elif index+1 < len(string) and string[index:index+2] in tokens:
                    item = string[index:index+2]
                    index += 2
                    self.items.append(item)
                    self.index = index
                elif index < len(string) and string[index] in tokens:
                    item = string[index]
                    index += 1
                    self.items.append(item)
                    self.index = index
                elif string[index] is '.':
                    if index+1 < len(string) and string[index+1] in digits:
                        item = self.read_numeric_literal()
                    else:
                        item = string[index]
                        index += 1
                        self.items.append(item)
                        self.index = index
                elif string[index] in digits:
                    item = self.read_numeric_literal()
                elif string[index] in ["'", '"']:
                    item = self.read_string_literal()
                else:
                    item = ''
                    while index < len(string) and string[index] in letters + digits + '_':
                        item += string[index]
                        index += 1
                    if item not in stringprefixes:
                        self.items.append(item)
                        self.index = index
                    elif item in stringprefixes and string[index] not in ["'", '"']:
                        self.items.append(item)
                        self.index = index
                    else:
                        item = self.read_string_literal(item)
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

def test():
    sc = scanner()
    while True:
        print '>>>',
        sc.get_line()
        items = []
        item = sc.read()
        while item != '':
            items.append(item)
            item = sc.read()
        print items

if __name__ == '__main__':
    test()