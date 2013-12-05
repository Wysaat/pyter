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
tokens.remove('.')

class lexical_analyzer(object):
    def __init__(self):
        self.items = []
        self.line_number = 0
        self.rewind = 0

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
                    backslash = 0
                else:
                    item += string[index]
                    if string[index] == '\\':
                        if backslash == 0:
                            if index == len(string) - 1:
                                item = item[:-1]
                            backslash = 1
                    elif string[index] == op:
                        backslash = 0
                        op_num += 1
                        if op_num == 3:
                            index += 1
                            self.index = index
                            self.items.append(item)
                            return item
                    else:
                        backslash = 0
                        op_num = 0
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
            return item
        else:
            return ''

    def read(self):
        if self.rewind == 1:
            self.rewind = 0
            return self.items[-1]
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

def is_id(item):
    if item in keywords:
        return False
    if item[0] in letters + '_':
        for i in range(len(item)):
            if item[i] not in letters + digits + '_':
                return False
        return True
    return False

def is_kw(item):
    if item in keywords:
        return True
    return False

def is_str(item):
    if len(item) < 2:
        return False
    if item[0] in ["'", '"']:
        return True
    if item[0] in stringprefixes and item[1] in ["'", '"']:
        return True
    if len(item) > 2 and item[:2] in stringprefixes and item[2] in ["'", '"']:
        return True
    return False

def is_num(item):
    if item[0] == '.' and len(item) > 1:
        return True
    if item[0] in digits:
        return True
    return False

def is_op(item):
    if item in operators:
        return True
    return False

def is_dot(item):
    if item is '.':
        return True
    return False

def is_dl(item):
    if item in delimiters:
        return True
    return False


la = lexical_analyzer()

def parse_atom():
    item = la.read()
    if is_id(item) or is_num(item) or is_str(item):
        return True
    if item is '(':
        item = la.read()
        if item == ')':
            return True
        if item == 'yield':
            parse_expression_list(')')
        else:
            la.rewind = 1
            parse_expression()
        item = la.read()
        if item == ',':
            item = la.read()
            if item == ')':
                return True
            la.rewind = 1
            parse_expression_list(')')
            item = la.read()
        elif item is 'for':
            la.rewind = 1
            parse_comp_for()
            item = la.read()
        if item == ')':
            return True
        else:
            la.syntax_error()
    if item is '[':
        item = la.read()
        if item == ']':
            return True
        la.rewind = 1
        parse_expression()
        if item is ',':
            item = la.read()
            if item == ']':
                return True
            la.rewind = 1
            parse_expression_list(')')
            item = la.read()
        elif item is 'for':
            la.rewind()
            parse_list_for(ending)
            item = la.read()
        if item == ']':
            return True
        else:
            la.syntax_error()
    if item is '{':
        item = la.read()
        if item == '}':
            return True
        parse_expression()
        item = la.read()
        if item == ':':
            parse_expression()
            item = la.read()
            if item == ',':
                item = la.read()
                if item != '}':
                    la.rewind = 1
                    parse_key_datum_list('}')
                    item = la.read()
            elif item == 'for':
                la.rewind = 1
                parse_comp_for()
                item = la.read()
        if item == '}':
            return True
        else:
            la.syntax_error()
    if item is '`':
        parse_expression_list('`')
        item = la.read()
        if item == '`':
            return True
        else:
            la.syntax_error()
    la.syntax_error()

def parse_primary():
    parse_atom()
    while True:
        item = la.read()
        if item == '.':
            item = la.read()
            if not is_id(item):
                la.syntax_error()
        elif item == '[':
            parse_slice_list(']')
            item = la.read()
            if item != ']':
                la.syntax_error()
        elif item == '(':
            item = la.read()
            la.rewind = 1
            if item == '*':
                parse_argument_list()
            elif item != ')':
                parse_expression()
                item = la.read()
                if item == 'for':
                    la.rewind = 1
                    parse_comp_for()
                elif item == ',':
                    item = la.read()
                    la.rewind = 1
                    if item != ')':
                        parse_argument_list()
        else:
            la.rewind = 1
            return True

# Trailing commas like: f(*arg, a, b) are allowed,
# as is specified in the doc.
def parse_argument_list():
    asterisk = 0
    while True:
        item = la.read()
        if item == '*' or is_id(item):
            if item == '*':
                asterisk = 1
            break
        if item == '**':
            parse_expression()
            item = la.read()
            if item != ',':
                la.rewind = 1
            return True
        la.rewind = 1
        parse_expression()
        item = la.read()
        if item != ',':
            return True
        else:
            item = la.read()
            la.rewind = 1
            if item == ')':
                return True
    while True:
        if is_id(item):
            item = la.read()
            if item != '=':
                la.syntax_error()
            parse_expression()
        elif item == '*':
            if asterisk == 1:
                la.syntax_error()
            asterisk = 1
            parse_expression()
        elif item == '**':
            parse_expression()
            item = la.read()
            if item != ',':
                la.rewind = 1
            return True
        item = la.read()
        if item != ',':
            return True
        else:
            item = la.read()
            la.rewind = 1
            if item == ')':
                return True

def parse_power():
    parse_primary()
    item = la.read()
    if item != '**':
        la.rewind = 1
        return True
    parse_u_expr()

def parse_u_expr():
    item = la.read()
    if item in '-+~':
        parse_u_expr()
    else:
        la.rewind = 1
        parse_power()

def parse_m_expr():
    parse_u_expr()
    while True:
        item = la.read()
        if item not in ['*', '//', '/', '%']:
            la.rewind = 1
            return True
        parse_u_expr()

def parse_a_expr():
    parse_m_expr()
    while True:
        item = la.read()
        if item not in '+-':
            la.rewind = 1
            return True
        parse_m_expr()

def parse_shift_expr():
    parse_a_expr()
    while True:
        item = la.read()
        if item not in ['<<', '>>']:
            la.rewind = 1
            return True
        parse_a_expr()

def parse_and_expr():
    parse_shift_expr()
    while True:
        item = la.read()
        if item != '&':
            la.rewind = 1
            return True
        parse_shift_expr()

def parse_xor_expr():
    parse_and_expr()
    while True:
        item = la.read()
        if item != '^':
            la.rewind = 1
            return True
        parse_and_expr()

def parse_or_expr():
    parse_xor_expr()
    while True:
        item = la.read()
        if item != '|':
            la.rewind = 1
            return True
        parse_xor_expr()

comp_operators = ['<', '>', '==', '>=', '<=', '<>', '!=',]

def parse_comparison():
    parse_or_expr()
    while True:
        item = la.read()
        if item == 'is':
            item = la.read()
            if item != 'not':
                la.rewind = 1
            parse_or_expr()
        if item == 'not':
            item = la.read()
            if item != 'in':
                la.syntax_error()
            parse_or_expr()
        elif item in comp_operators:
            parse_or_expr()
        else:
            la.rewind = 1
            return True

def parse_not_test():
    item = la.read()
    if item == 'not':
        parse_not_test()
    else:
        la.rewind = 1
        parse_comparison()

def parse_and_test():
    parse_not_test()
    while True:
        item = la.read()
        if item != 'and':
            la.rewind = 1
            return True
        parse_not_test()

def parse_or_test():
    parse_and_test()
    while True:
        item = la.read()
        if item != 'or':
            la.rewind = 1
            return True
        parse_and_test()

def parse_conditional_expression():
    parse_or_test()
    item = la.read()
    if item != 'if':
        la.rewind = 1
        return True
    parse_or_test()
    item = la.read()
    if item != 'else':
        la.syntax_error()
    parse_expression()

def parse_expression():
    item = la.read()
    if item == 'lambda':
        parse_lambda_expr()
    else:
        la.rewind = 1
        parse_conditional_expression()

# lambda expressions cannot contain annotations(Python 3) and statements
def parse_lambda_expr():
    item = la.read()
    if item != 'lambda':
        la.syntax_error()

# expression_list is a subset of slice_list
# short_slice is a subset of slice_list
# So, subscriptions and slicings are both parsed
# by this single function.
def parse_slice_list(ending):
    item = la.read()
    if item == Ellipsis:
        return True
    if item != ':':
        la.rewind = 1
        parse_expression()
        item = la.read()
        if item == ending:
            la.rewind = 1
            return True
        elif item == ',':
            item = la.read()
            la.rewind = 1
            if item == ending:
                return True
            else:
                parse_slice_list(ending)
    if item == ':':
        item = la.read()
        if item not in [':', ',', ending]:
            la.rewind = 1
            parse_expression()
            item = la.read()
        if item == ':':
            item = la.read()
            if item not in [',', ending]:
                la.rewind = 1
                parse_expression()
                item = la.read()
        if item == ',':
            item = la.read()
            la.rewind = 1
            if item != ending:
                parse_slice_list(ending)
            else:
                return True
        if item == ending:
            la.rewind = 1
            return True

def parse_key_datum_list(ending):
    parse_expression()
    item = la.read()
    if item != ':':
        la.syntax_error()
    parse_expression()
    item = la.read()
    if item == ending:
        la.rewind = 1
        return True
    elif item == ',':
        item = la.read()
        la.rewind = 1
        if item == ending:
            return True
        else:
            parse_key_datum_list()

def parse_list_for(ending):
    item = la.read()
    if item != 'for':
        la.syntax_error()
    parse_target_list()
    item = la.read()
    if item != 'in':
        la.syntax_error()
    parse_expression_nocond()
    item = la.read()
    if item == ',':
        while True:
            parse_expression_nocond()
            item = la.read()
            if item != ',':
                break
            item = la.read()
            la.rewind = 1
            if item in ['for', 'if', ending]:
                break
    la.rewind = 1
    if item == 'for':
        parse_list_for(ending)
    elif item == 'if':
        parse_list_if(ending)
    else:
        return True

def parse_list_if(ending):
    item = la.read()
    if item != 'if':
        la.syntax_error()
    parse_expression_nocond()
    item = la.read()
    la.rewind = 1
    if item == 'for':
        parse_list_for(ending)
    elif item == 'if':
        parse_list_if(ending)
    else:
        return True

def parse_comp_for():
    item = la.read()
    if item != 'for':
        la.syntax_error()
    parse_target_list()
    item = la.read()
    if item != 'if':
        la.syntax_error()
    parse_or_test()
    item = la.read()
    la.rewind = 1
    if item == 'for':
        parse_comp_for()
    elif item == 'if':
        parse_comp_if()
    else:
        return True

def parse_comp_if():
    item = la.read()
    if item != 'if':
        la.syntax_error()
    parse_expression_nocond()
    item = la.read()
    la.rewind = 1
    if item == 'for':
        parse_comp_for()
    elif item == 'if':
        parse_comp_if()
    else:
        return True

def parse_expression_list(ending):
    parse_expression()
    item = la.read()
    if item == ',':
        item = la.read()
        la.rewind = 1
        if item != ending:
            parse_expression_list(ending)
    la.rewind = 1
    return True

def parse_target_list():
    pass