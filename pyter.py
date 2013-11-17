# from __future__ import division
# import operator
# import math
# import copy
# 
# env = vars(math)
# env.update(vars(operator))

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

    def read(self):
        string = self.string
        index = self.index
        if index < len(string):
            while string[index] in [' ', '\t',]:
                index += 1
            if string[index:index+3] in operators:
                item = string[index:index+3]
                index += 3
            elif string[index:index+2] in operators:
                item = string[index:index+2]
                index += 2
            elif string[index] in operators:
                item = string[index]
                index += 1
            else:
                item = ''
                while index < len(string) and string[index] not in [' ', '\t',] + operators \
                      and string[index:index+2] not in operators:
                    item += string[index]
                    index += 1
            self.items.append(item)
            self.index = index
            return item
        else:
            return ''

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
            item = self.read()
            if item is ']' and comma == 1:
                return
            else:
                self.scan_noun(item)
            item = self.read()
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
        sc.get_line()
        sc.scan_noun()

if __name__ == '__main__':
    test()