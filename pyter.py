# references: Python 2.7 language reference
#             Python 3.3 languare reference
from string import letters
import sys

digits = '0123456789'

stringprefixes = ['r', 'R', 'u', 'U', 'ur', 'uR', 'Ur', 'UR',
                  'b', 'B', 'br', 'bR', 'Br', 'BR']

# confirm to Python 3, 'print' is no more a keyword
keywords = ['and', 'as', 'assert', 'break', 'class', 'continue', 'def',
            'del', 'elif', 'else', 'except', 'exec', 'finally', 'for',
            'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'not',
            'or', 'pass', 'raise', 'return', 'try','while',
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

aug_ops = ['+=', '-=', '*=', '/=', '//=', '%=', '**=',
           '>>=', '<<=', '&=', '^=', '|=',]

tokens = operators + delimiters + other_tokens
tokens.remove('.')

INDENT = hash('INDENT')
DEDENT = hash('DEDENT')
EOF = hash('EOF')

compound_statement_starts = ['if', 'while', 'for', 'try', 'with', '@', 'def', 'class',]

class lexical_analyzer(object):
    def __init__(self):
        self.items = []
        self.line_number = 0
        self.multi_lines = 0
        self.pass_empty_lines = 1
        self._rewind = 0
        self.file = None
        self.interactive = 0
        if len(sys.argv) > 1:
            self.filename = sys.argv[1]
            self.file = open(self.filename)
            self.interactive = 1

        self.indentation_stack = [0,]
        self.indentation = 0
        self.eof = 0
        self.dedents = 0
        # end of block flag for interactive input
        self.eob = 0
        # interactive input
        self.symbol = '>>>'
        self.in_block = 0
        self.eol = 0
        self.line_joining = 0

        # DANGEROUS to put a function here!!! fix it some day...
        self.get_line()

# BUGGY
    def get_line(self):
        if self.eof:
            self.string = ''
            return
        self.eol = 0
        if len(sys.argv) > 1:
            self.string = self.file.readline()
            while self.string.strip() == '' or self.string.strip()[0] == '#':
                if self.string == '' or self.string[-1] != '\n':
                    self.eof = 1
                    if self.string != '' and self.string.strip()[0] == '#':
                        self.string = ''
                    break
                self.line_number += 1
                self.string = self.file.readline()
            if self.string != '' and self.string[-1] == '\n':
                self.string = self.string[:-1]
        else:
            self.filename = '<stdin>'
            if self.multi_lines or self.in_block or self.line_joining:
                self.symbol = '...'
            else:
                self.symbol = '>>>'
            print self.symbol,
            self.line_joining = 0
            self.string = raw_input()
            if self.in_block:
                while self.string != '' and self.string.strip() == '':
                    print self.symbol,
                    self.line_number += 1
                    self.string = raw_input()
                if self.string == '':
                    self.eob = 1
            else:
                while self.string.strip() == '' or self.string.strip()[0] == '#':
                    if self.string == '' and not self.multi_lines:
                        print '>>>',
                        self.line_number = 0
                    else:
                        print '...',
                        self.line_number += 1
                    self.string = raw_input()
        self.index = 0
        self.line_number += 1

    def error(self, string):
        if self._rewind == 1:
            self_string = self.last_string
            self_index = self.last_index
        else:
            self_string = self.string
            self_index = self.index
        print '  File "' + self.filename + '", line ' + str(self.line_number)
        print '    ' + self_string
        print '    ' + ' ' * (self_index - 1) + '^'
        print string
        exit()

    def syntax_error(self):
        self.error('syntax_error: invalid syntax')

    def raw_error(self, string):
        print '  File "' + self.filename + '", line ' + str(self.line_number)
        print string
        exit()

    def read_string_literal(self, item=''):
        backslash = 0
        op = self.string[self.index]
        if self.string[self.index:self.index+3] in ['"""', "'''"]:
            self.multi_lines += 1
            item += self.string[self.index:self.index+3]
            op_num = 0
            self.index += 3
            while True:
                if self.eof:
                    self.error('EOF while scanning triple-quoted string literal')  
                if self.index == len(self.string):
                    self.get_line()
                    self.index = 0
                    op_num = 0
                    if backslash == 0:
                        item += '\n'
                    backslash = 0
                else:
                    item += self.string[self.index]
                    if self.string[self.index] == '\\':
                        if backslash == 0:
                            if self.index == len(self.string) - 1:
                                item = item[:-1]
                            backslash = 1
                    elif self.string[self.index] == op:
                        backslash = 0
                        op_num += 1
                        if op_num == 3:
                            self.index += 1
                            self.items.append(item)
                            self.multi_lines -= 1
                            return item
                    else:
                        backslash = 0
                        op_num = 0
                    self.index += 1
        elif self.string[self.index] in ['"', "'"]:
            item += self.string[self.index]
            self.index += 1
            while True:
                while self.index < len(self.string):
                    item += self.string[self.index]
                    if backslash == 0 and self.string[self.index] == op:
                        self.index += 1
                        self.items.append(item)
                        return item
                    elif backslash == 0 and self.string[self.index] == '\\':
                        backslash = 1
                    else:
                        backslash = 0
                    self.index += 1
                if backslash == 1:
                    item = item[:-1]
                    self.multi_lines += 1
                    self.get_line()
                    self.multi_lines -= 1
                    self.index = 0
                    backslash = 0
                else:
                    self.error('syntax_error: EOL while scanning string literal')

    def read_numeric_literal(self):
        digits = '0123456789'
        item = ''
        is_float = 0
        is_imaginary = 0
        other_starts = ['0' + digit for digit in digits]
        if self.index+1 < len(self.string) and self.string[self.index:self.index+2] in ['0x', '0X', '0o', '0O', '0b', '0B']:
            item += self.string[self.index:self.index+2]
            if item in ['0b', '0B']:
                digits = '01'
            elif item in ['0o', '0O']:
                digits = '01234567'
            elif item in ['0x', '0X']:
                digits = '0123456789abcdefABCDEF'
            self.index += 2
            if self.index == len(self.string):
                self.error('syntax_error: invalid token')
            elif self.string[self.index] not in digits:
                self.error('syntax_error: invalid token')
            while self.string[self.index] in digits:
                item += self.string[self.index]
                self.index += 1
                if self.index == len(self.string):
                    self.items.append(item)
                    return item
            if self.string[self.index] in ['l', 'L']:
                item += self.string[self.index]
                self.index += 1
            self.items.append(item)
            return item
        if self.index+1 < len(self.string) and self.string[self.index:self.index+2] in other_starts:
            invalid = 0
            while self.index < len(self.string) and self.string[self.index] in digits:
                item += self.string[self.index]
                if int(self.string[self.index]) > 7:
                    invalid = 1
                self.index += 1
            if invalid:
                if self.index == len(self.string):
                    self.error('syntax_error: invalid token')
                elif self.string[self.index] not in '.eEjJ':
                    self.error('syntax_error: invalid token')
            if self.index == len(self.string):
                self.items.append(item)
                return item
        if self.string[self.index] is '.':
            item += self.string[self.index]
            self.index += 1
            is_float = 1
            if self.index == len(self.string):
                self.items.append(item)
                return item
        elif self.string[self.index] in digits:
            tmp_ind = self.index
            while tmp_ind < len(self.string) and self.string[tmp_ind] in digits:
                tmp_ind += 1
            if tmp_ind < len(self.string) and self.string[tmp_ind] is '.':
                item += self.string[self.index:tmp_ind+1]
                self.index = tmp_ind + 1
                is_float = 1
                if self.index == len(self.string):
                    self.items.append(item)
                    return item
        while self.string[self.index] in digits:
            item += self.string[self.index]
            self.index += 1
            if self.index == len(self.string):
                self.items.append(item)
                return item
        if self.string[self.index] in 'lL':
            if not is_float:
                item += self.string[self.index]
                self.index += 1
            self.items.append(item)
            return item
        elif self.string[self.index] in 'jJ':
            item += self.string[self.index]
            self.index += 1
            self.items.append(item)
            return item
        elif self.string[self.index] not in 'eE':
            self.items.append(item)
            return item
        else:
            item += self.string[self.index]
            self.index += 1
            if self.index == len(self.string):
                self.error('syntax_error: invalid token')
            if self.string[self.index] not in digits + '+-':
                self.error('syntax_error: invalid token')
            if self.string[self.index] in ['+', '-']:
                item += self.string[self.index]
                self.index += 1
                if self.index == len(self.string):
                    self.error('syntax_error: invalid token')
                if self.string[self.index] not in digits:
                    syntax_error('syntax_error: invalid token')
            while self.string[self.index] in digits:
                item += self.string[self.index]
                self.index += 1
                if self.index == len(self.string):
                    self.items.append(item)
                    return item
            if self.string[self.index] in 'jJ':
                item += self.string[self.index]
                self.index += 1
            self.items.append(item)
            return item

    # the indent/dedent part is BUGGY, to be fixed
    def raw_read(self):
        digits = '0123456789'
        self.indentation = 0
        if self.eob:
            self.dedents += len(self.indentation_stack) - 1
            self.indentation_stack = [0]
            self.eob = 0
        if self.dedents > 0:
            self.dedents -= 1
            self.items.append(DEDENT)
            return DEDENT
        if self.eof and self.string.strip() == '':
            if len(self.indentation_stack) > 1:
                self.dedents += len(self.indentation_stack) - 1
                self.indentation_stack = [0]
                self.dedents -= 1
                self.items.append(DEDENT)
                return DEDENT
            self.items.append(EOF)
            return EOF
        if self.index < len(self.string):
            if self.string[self.index:].strip() == '':
                item = ''
                # for printing error message, 1 more than true self.index
                self.index = len(self.string)
                self.items.append(item)
                self.eol = 1
            else:
                if self.index == 0 and not self.multi_lines:
                    while self.string[self.index] in [' ', '\t',]:
                        self.indentation += 1
                        self.index += 1
                    if self.indentation > self.indentation_stack[-1]:
                        self.indentation_stack.append(self.indentation)
                        self.items.append(INDENT)
                        return INDENT
                    elif self.indentation < self.indentation_stack[-1]:
                        for i in range(len(self.indentation_stack)-1, -1, -1):
                            if self.indentation > self.indentation_stack[i]:
                                la.error('indentation_error: unindent does not match any outer indentation level')
                            elif self.indentation == self.indentation_stack[i]:
                                self.dedents -= 1
                                self.items.append(DEDENT)
                                return DEDENT
                            self.dedents += 1
                            self.indentation_stack.pop()
                else:
                    while self.string[self.index] in [' ', '\t',]:
                        self.index += 1
                if self.string[self.index] == '#':
                    item = ''
                    self.items.append(item)
                    self.eol = 1
                    return item
                elif self.index+2 < len(self.string) and self.string[self.index:self.index+3] in tokens:
                    item = self.string[self.index:self.index+3]
                    self.index += 3
                    self.items.append(item)
                elif self.index+1 < len(self.string) and self.string[self.index:self.index+2] in tokens:
                    item = self.string[self.index:self.index+2]
                    self.index += 2
                    self.items.append(item)
                elif self.index < len(self.string) and self.string[self.index] in tokens:
                    item = self.string[self.index]
                    self.index += 1
                    self.items.append(item)
                elif self.string[self.index] is '.':
                    if self.index+1 < len(self.string) and self.string[self.index+1] in digits:
                        item = self.read_numeric_literal()
                    else:
                        item = self.string[self.index]
                        self.index += 1
                        self.items.append(item)
                elif self.string[self.index] in digits:
                    item = self.read_numeric_literal()
                elif self.string[self.index] in ["'", '"']:
                    item = self.read_string_literal()
                else:
                    item = ''
                    while self.index < len(self.string) and self.string[self.index] in letters + digits + '_':
                        item += self.string[self.index]
                        self.index += 1
                    if self.index == len(self.string):
                        self.items.append(item)
                    elif item not in stringprefixes:
                        self.items.append(item)
                    elif item in stringprefixes and self.string[self.index] not in ["'", '"']:
                        self.items.append(item)
                    else:
                        item = self.read_string_literal(item)
            return item
        else:
            item = ''
            self.items.append(item)
            self.eol = 1
            return item

    # EOF should be appended to items,
    # or cannot rewind it.
    def rewind(self):
        self._rewind = 1
        self.last_item = self.items.pop()

    def read(self):
        self.last_string = self.string
        self.last_index = self.index
        self.last_line_number = self.line_number
        if self._rewind == 1:
            self._rewind = 0
            self.items.append(self.last_item)
            return self.last_item
        if self.eol:
            self.get_line()
        item = self.raw_read()
        if item == '\\':
            self.items.pop()
            if self.raw_read() != '':
                self.error('syntax_error: unexpected character after line continuation character')
            else:
                self.items.pop()
                self.line_joining = 1
                self.get_line()
                item = self.read()
                return item
        if item == '' and self.multi_lines:
            self.items.pop()
            item = self.read()
        return item

def is_id(item):
    if type(item) != str:
        return False

    if item == '':
        return False
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
    if type(item) != str:
        return False

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
    if type(item) != str:
        return False

    if item == '':
        return False
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
    if is_id(item) or is_num(item):
        return True
    if is_str(item):
        while is_str(item):
            item = la.read()
        la.rewind()
        return True
    if item is '(':
        la.multi_lines += 1
        item = la.read()
        if item == ')':
            la.multi_lines -= 1
            return True
        if item == 'yield':
            parse_expression_list(')')
        else:
            la.rewind()
            parse_expression()
        item = la.read()
        if item == ',':
            item = la.read()
            if item == ')':
                la.multi_lines -= 1
                return True
            la.rewind()
            parse_expression_list(')')
            item = la.read()
        elif item == 'for':
            la.rewind()
            parse_comp_for()
            item = la.read()
        if item == ')':
            la.multi_lines -= 1
            return True
        else:
            la.syntax_error()
    if item is '[':
        la.multi_lines += 1
        item = la.read()
        if item == ']':
            la.multi_lines -= 1
            return True
        la.rewind()
        parse_expression()
        item = la.read()
        if item is ',':
            item = la.read()
            if item == ']':
                la.multi_lines -= 1
                return True
            la.rewind()
            parse_expression_list(']')
            item = la.read()
        elif item == 'for':
            la.rewind()
            parse_list_for(']')
            item = la.read()
        if item == ']':
            la.multi_lines -= 1
            return True
        else:
            la.syntax_error()
    if item is '{':
        la.multi_lines += 1
        item = la.read()
        if item == '}':
            la.multi_lines -= 1
            return True
        la.rewind()
        parse_expression()
        item = la.read()
        if item == ':':
            parse_expression()
            item = la.read()
            if item == ',':
                item = la.read()
                if item != '}':
                    la.rewind()
                    parse_key_datum_list('}')
                    item = la.read()
            elif item == 'for':
                la.rewind()
                parse_comp_for()
                item = la.read()
        elif item == ',':
            parse_expression_list('}')
            item = la.read()
        elif item == 'for':
            la.rewind()
            parse_comp_for()
            item = la.read()
        if item == '}':
            la.multi_lines -= 1
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
            la.multi_lines += 1
            parse_slice_list(']')
            item = la.read()
            if item != ']':
                la.syntax_error()
            la.multi_lines -= 1
        elif item == '(':
            la.multi_lines += 1
            item = la.read()
            if item in ['*', '**']:
                la.rewind()
                parse_argument_list()
                item = la.read()
                if item != ')':
                    la.syntax_error()
            elif item != ')':
                la.rewind()
                parse_expression()
                item = la.read()
                if item == 'for':
                    la.rewind()
                    parse_comp_for()
                    item = la.read()
                    if item != ')':
                        la.syntax_error()
                elif item == ',':
                    item = la.read()
                    if item != ')':
                        la.rewind()
                        parse_argument_list()
                        item = la.read()
                        if item != ')':
                            la.syntax_error()
                elif item == '=':
                    parse_expression()
                    item = la.read()
                    if item == ',':
                        item = la.read()
                        if item != ')':
                            la.rewind()
                            parse_argument_list()
                            item = la.read()
                            if item != ')':
                                la.syntax_error()
                    elif item != ')':
                       la.syntax_error()
                elif item != ')':
                    la.syntax_error()
            la.multi_lines -= 1
        else:
            la.rewind()
            return True

# Trailing commas like: f(*args, a, b) are allowed,
# as is specified in the doc.
def parse_argument_list():
    asterisk = 0
    while True:
        item = la.read()
        if item == '*':
            parse_expression()
            asterisk = 1
            break
        if item == '**':
            parse_expression()
            item = la.read()
            if item != ',':
                la.rewind()
            return True
        la.rewind()
        parse_expression()
        item = la.read()
        if item == '=':
            parse_expression()
            break
        elif item != ',':
            la.rewind()
            return True
        else:
            item = la.read()
            la.rewind()
            if item == ')':
                return True
    while True:
        item = la.read()
        if item != ',':
            la.rewind()
            return True
        else:
            item = la.read()
            la.rewind()
            if item == ')':
                return True
        item = la.read()
        if item == '*':
            if asterisk:
                la.syntax_error()
            parse_expression()
            break
        if item == '**':
            parse_expression()
            item = la.read()
            if item != ',':
                la.rewind()
            return True
        la.rewind()
        parse_expression()
        item = la.read()
        if item != '=':
            la.raw_error('syntax_error: only named arguments may follow *expression')
        parse_expression()
    while True:
        item = la.read()
        if item != ',':
            la.rewind()
            return True
        else:
            item = la.read()
            la.rewind()
            if item == ')':
                return True
        item = la.read()
        if item == '**':
            parse_expression()
            item = la.read()
            if item != ',':
                la.rewind()
            return True
        la.rewind()
        parse_expression()
        item = la.read()
        if item != '=':
            la.raw_error('syntax_error: only named arguments may follow *expression')
        parse_expression()

def parse_power():
    parse_primary()
    item = la.read()
    if item != '**':
        la.rewind()
        return True
    parse_u_expr()

def parse_u_expr():
    item = la.read()
    # BUG: if item in '-+~'
    # REASON: '' in '-+~'
    if item in ['-', '+', '~']:
        parse_u_expr()
    else:
        la.rewind()
        parse_power()

def parse_m_expr():
    parse_u_expr()
    while True:
        item = la.read()
        if item not in ['*', '//', '/', '%']:
            la.rewind()
            return True
        parse_u_expr()

def parse_a_expr():
    parse_m_expr()
    while True:
        item = la.read()
        if item not in ['+', '-']:
            la.rewind()
            return True
        parse_m_expr()

def parse_shift_expr():
    parse_a_expr()
    while True:
        item = la.read()
        if item not in ['<<', '>>']:
            la.rewind()
            return True
        parse_a_expr()

def parse_and_expr():
    parse_shift_expr()
    while True:
        item = la.read()
        if item != '&':
            la.rewind()
            return True
        parse_shift_expr()

def parse_xor_expr():
    parse_and_expr()
    while True:
        item = la.read()
        if item != '^':
            la.rewind()
            return True
        parse_and_expr()

def parse_or_expr():
    parse_xor_expr()
    while True:
        item = la.read()
        if item != '|':
            la.rewind()
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
                la.rewind()
            parse_or_expr()
        elif item == 'not':
            item = la.read()
            if item != 'in':
                la.syntax_error()
            parse_or_expr()
        elif item == 'in':
            parse_or_expr()
        elif item in comp_operators:
            parse_or_expr()
        else:
            la.rewind()
            return True

def parse_not_test():
    item = la.read()
    if item == 'not':
        parse_not_test()
    else:
        la.rewind()
        parse_comparison()

def parse_and_test():
    parse_not_test()
    while True:
        item = la.read()
        if item != 'and':
            la.rewind()
            return True
        parse_not_test()

def parse_or_test():
    parse_and_test()
    while True:
        item = la.read()
        if item != 'or':
            la.rewind()
            return True
        parse_and_test()

def parse_conditional_expression():
    parse_or_test()
    item = la.read()
    if item != 'if':
        la.rewind()
        return True
    parse_or_test()
    item = la.read()
    if item != 'else':
        la.syntax_error()
    parse_expression()

def parse_expression():
    item = la.read()
    la.rewind()
    if item == 'lambda':
        parse_lambda_expr()
    else:
        parse_conditional_expression()

def parse_expression_nocond():
    item = la.read()
    la.rewind()
    if item == 'lambda':
        parse_lambda_expr()
    else:
        parse_or_test()

# lambda expressions cannot contain annotations(Python 3) or statements
def parse_lambda_expr():
    item = la.read()
    if item != 'lambda':
        la.syntax_error()
    parse_lambda_parameter_list()
    item = la.read()
    if item != ':':
        la.syntax_error()
    parse_expression()

def parse_lambda_expr_nocond():
    item = la.read()
    if item != 'lambda':
        la.syntax_error()
    parse_lambda_parameter_list()
    item = la.read()
    if item != ':':
        la.syntax_error()
    parse_expression_nocond()

# conform to Python 3
def parse_lambda_parameter_list():
    asterisk = 0
    while True:
        item = la.read()
        if item == ':':
            la.rewind()
            return True
        if is_id(item):
            item = la.read()
            if item == '=':
                parse_expression()
            else:
                la.rewind()
            item = la.read()
            if item == ':':
                la.rewind()
                return True
            elif item != ',':
                la.syntax_error()
        elif item == '*':
            if asterisk == 1:
                la.syntax_error()
            asterisk = 1
            item = la.read()
            if is_id(item):
                item = la.read()
            if item == ':':
                la.rewind()
                return True
            elif item != ',':
                la.syntax_error()
        elif item == '**':
            item = la.read()
            if not is_id(item):
                la.syntax_error()
            return True
        else:
            la.syntax_error()

def parse_star_expr_list(*endings):
    while True:
        parse_star_expr()
        item = la.read()
        if item == ',':
            item = la.read()
            la.rewind()
            if item in endings:
                return True
        else:
            la.rewind()
            return True

def parse_target_list(*endings):
    while True:
        item = la.read()
        if item != '*':
            la.rewind()
        parse_primary()
        item = la.read()
        if item != ',':
            la.rewind()
            return True
        item = la.read()
        la.rewind()
        if item in endings:
            return True

def parse_star_expr():
    item = la.read()
    if item != '*':
        la.rewind()
    parse_expression()

# expression_list is a subset of slice_list
# short_slice is a subset of slice_list
# So, subscriptions and slicings are both parsed
# by this single function.
def parse_slice_list(ending):
    item = la.read()
    if item == '.':
        item = la.read()
        if item == '.':
            item = la.read()
            if item == '.':
                return True
        la.syntax_error()
    if item != ':':
        la.rewind()
        parse_expression()
        item = la.read()
        if item == ending:
            la.rewind()
            return True
        elif item == ',':
            item = la.read()
            la.rewind()
            if item == ending:
                return True
            else:
                parse_slice_list(ending)
    if item == ':':
        item = la.read()
        if item not in [':', ',', ending]:
            la.rewind()
            parse_expression()
            item = la.read()
        if item == ':':
            item = la.read()
            if item not in [',', ending]:
                la.rewind()
                parse_expression()
                item = la.read()
        if item == ',':
            item = la.read()
            la.rewind()
            if item != ending:
                parse_slice_list(ending)
            else:
                return True
        if item == ending:
            la.rewind()
            return True

def parse_key_datum_list(ending):
    parse_expression()
    item = la.read()
    if item != ':':
        la.syntax_error()
    parse_expression()
    item = la.read()
    if item == ending:
        la.rewind()
        return True
    elif item == ',':
        item = la.read()
        la.rewind()
        if item == ending:
            return True
        else:
            parse_key_datum_list(ending)

def parse_list_for(ending):
    item = la.read()
    if item != 'for':
        la.syntax_error()
    parse_target_list('in')
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
            if item in ['for', 'if', ending]:
                break
            la.rewind()
    la.rewind()
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
    la.rewind()
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
    parse_target_list('in')
    item = la.read()
    if item != 'in':
        la.syntax_error()
    parse_or_test()
    item = la.read()
    la.rewind()
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
    la.rewind()
    if item == 'for':
        parse_comp_for()
    elif item == 'if':
        parse_comp_if()
    else:
        return True

def parse_expression_list(*endings):
    while True:
        parse_expression()
        item = la.read()
        if item == ',':
            item = la.read()
            la.rewind()
            if item in endings:
                return True
        else:
            la.rewind()
            return True

##############################################
########## PARSE SIMPLE STATEMENTS ###########
##############################################

def parse_simple_stmt():
    item = la.read()
    if item == 'assert':
        parse_expression_list('', ';')
    elif item == 'pass':
        return True
    elif item == 'del':
        parse_expression_list('', ';')
    elif item == 'return' or item == 'print':
        item = la.read()
        la.rewind()
        if item in ['', ';']:
            return True
        parse_expression_list('', ';')
    elif item == 'yield':
        parse_expression_list('', ';')
    elif item == 'raise':
        item = la.read()
        la.rewind()
        if item in ['', ';']:
            return True
        parse_expression()
        item = la.read()
        if item == 'from':
            parse_expression()
        else:
            la.rewind()
            return True
    elif item == 'break':
        return True
    elif item == 'continue':
        return True
    elif item == 'import':
        while True:
            parse_module()
            item = la.read()
            if item == 'as':
                item = la.read()
                if not is_id(item):
                    la.syntax_error()
                item = la.read()
            if item != ',':
                la.rewind()
                return True
    elif item == 'from':
        wildcard = 0
        item = la.read()
        if item != '.':
            la.rewind()
            parse_module()
            wildcard = 1
        else:
            while True:
                item = la.read()
                if item == 'import':
                    la.rewind()
                    break
                elif item != '.':
                    la.rewind()
                    parse_module()
                    break
        item = la.read()
        if item != 'import':
            la.syntax_error()
        item = la.read()
        if wildcard and item == '*':
            return True
        in_parentheses = 0
        if item != '(':
            la.rewind()
        else:
            in_parentheses = 1
            la.multi_lines += 1
        while True:
            item = la.read()
            if not is_id(item):
                la.syntax_error()
            item = la.read()
            if item == 'as':
                item = la.read()
                if not is_id(item):
                    la.syntax_error()
                item = la.read()
            if item == ',':
                if in_parentheses:
                    item = la.read()
                    if item == ')':
                        la.multi_lines -= 1
                        return True
                    la.rewind()
            elif item == ')':
                if in_parentheses:
                    la.multi_lines -= 1
                    return True
                la.rewind()
                return True
            else:
                if in_parentheses:
                    la.syntax_error()
                la.rewind()
                return True
    elif item == 'global':
        while True:
            item = la.read()
            if not is_id(item):
                la.syntax_error()
            item = la.read()
            if item != ',':
                la.rewind()
                return True
    elif item == 'nonlocal':
        while True:
            item = la.read()
            if not is_id(item):
                la.syntax_error()
            item = la.read()
            if item != ',':
                la.rewind()
                return True
    else:
        la.rewind()
        parse_star_expr_list('=', '', ';', *aug_ops)
        item = la.read()
        if item == '=':
            item = la.read()
            if item == 'yield':
                parse_expression_list('', ';')
            else:
                la.rewind()
                parse_expression_list('', ';')
        elif item in aug_ops:
            item = la.read()
            if item == 'yield':
                parse_expression_list('', ';')
            else:
                la.rewind()
                parse_expression_list('', ';')
        else:
            la.rewind()
            return True

def parse_module():
    while True:
        item = la.read()
        if not is_id(item):
            la.syntax_error()
        item = la.read()
        if item != '.':
            la.rewind()
            return True

#############################################
######### PARSE COMPOUND STATEMENTS #########
#############################################

def parse_compound_statement():
    item = la.read()
    if item == 'if':
        parse_expression()
        item = la.read()
        if item != ':':
            la.syntax_error()
        parse_suite()
        while True:
            item = la.read()
            if item == 'elif':
                parse_expression()
                item = la.read()
                if item != ':':
                    la.syntax_error()
                parse_suite()
            elif item == 'else':
                item = la.read()
                if item != ':':
                    la.syntax_error()
                parse_suite()
                return True
            else:
                la.rewind()
                return True
    elif item == 'while':
        parse_expression()
        item = la.read()
        if item != ':':
            la.syntax_error()
        parse_suite()
        item = la.read()
        if item == 'else':
            item = la.read()
            if item != ':':
                la.syntax_error()
            parse_suite()
            return True
        else:
            la.rewind()
            return True
    elif item == 'for':
        parse_target_list('in')
        item = la.read()
        if item != 'in':
            la.syntax_error()
        parse_expression_list(':')
        item = la.read()
        if item != ':':
            la.syntax_error()
        parse_suite()
        item = la.read()
        if item == 'else':
            item = la.read()
            if item != ':':
                la.syntax_error()
            parse_suite()
            return True
        else:
            la.rewind()
            return True
    elif item == 'try':
        item = la.read()
        if item != ':':
            la.syntax_error()
        parse_suite()
        item = la.read()
        if item == 'except':
            while True:
                item = la.read()
                if item != ':':
                    la.rewind()
                    parse_expression()
                    item = la.read()
                    if item == 'as':
                        parse_star_expr()
                        item = la.read()
                        if item != ':':
                            la.syntax_error()
                    elif item != ':':
                        la.syntax_error()
                parse_suite()
                item = la.read()
                if item != 'except':
                    break
            if item == 'else':
                item = la.read()
                if item != ':':
                    la.syntax_error()
                parse_suite()
                return True
            elif item == 'finally':
                item = la.read()
                if item != ':':
                    la.syntax_error()
                parse_suite()
                return True
            else:
                la.rewind()
                return True
        elif item == 'finally':
            item = la.read()
            if item != ':':
                la.syntax_error()
            parse_suite()
            return True
        else:
            la.syntax_error()
    elif item == 'with':
        while True:
            parse_expression()
            item = la.read()
            if item == 'as':
                parse_star_expr()
                item = la.read()
            if item != ',':
                if item == ':':
                    parse_suite()
                    return True
                else:
                    la.syntax_error()
    elif item == '@':
        la.rewind()
        parse_decorators()
        item = la.read()
        if item == 'def':
            la.rewind()
            parse_funcdef()
        elif item == 'class':
            la.rewind()
            parse_classdef()
        else:
            la.syntax_error()
    elif item == 'def':
        la.rewind()
        parse_funcdef()
    elif item == 'class':
        la.rewind()
        parse_classdef()
    else:
        la.syntax_error()

# without decorators
def parse_funcdef():
    item = la.read()
    if item != 'def':
        la.syntax_error()
    item = la.read()
    if not is_id(item):
        la.syntax_error()
    item = la.read()
    if item != '(':
        la.syntax_error()
    la.multi_lines += 1
    item = la.read()
    if item != ')':
        la.rewind()
        parse_parameter_list(')')
        item = la.read()
        if item != ')':
            la.syntax_error()
    la.multi_lines -= 1
    item = la.read()
    if item == '-':
        item = la.read()
        if item != '>':
            la.syntax_error()
        parse_expression()
        item = la.read()
    if item != ':':
        la.syntax_error()
    parse_suite()

def parse_classdef():
    item = la.read()
    if item != 'class':
        la.syntax_error()
    item = la.read()
    if not is_id(item):
        la.syntax_error()
    item = la.read()
    if item == '(':
        la.rewind()
        la.multi_lines += 1
        parse_inheritance()
        la.multi_lines -= 1
        item = la.read()
    if item != ':':
        la.syntax_error()
    parse_suite()

# CAUTION: the Python 3.3 language reference here is wrong,
# the inheritance should be an expression_list not a parameter_list.
def parse_inheritance():
    item = la.read()
    if item != '(':
        la.syntax_error()
    item = la.read()
    if item == ')':
        return True
    la.rewind()
    parse_expression_list(')')
    item = la.read()
    if item == ')':
        return True
    else:
        la.syntax_error()

def parse_decorators():
    while True:
        parse_decorator()
        item = la.read()
        la.rewind()
        if item != '@':
            return True

# not allowing trailing comma: @f(a,) is illegal
def parse_decorator():
    item = la.read()
    if item != '@':
        la.syntax_error()
    parse_dotted_name()
    item = la.read()
    if item == '':
        return True
    elif item == '(':
        item = la.read()
        if item == ')':
            item = la.read()
            if item == '':
                return True
            else:
                la.syntax_error()
        else:
            la.rewind()
            parse_parameter_list(')')
            item = la.read()
            if item != ')':
                la.syntax_error()
            item = la.read()
            if item != '':
                la.syntax_error()
    else:
        la.syntax_error()

def parse_dotted_name():
    while True:
        item = la.read()
        if not is_id(item):
            la.syntax_error()
        item = la.read()
        if item != '.':
            la.rewind()
            return True


# truly BUGGY!!!
def parse_parameter_list(ending):
    asterisk = 0
    while True:
        item = la.read()
        if item == '**':
            parse_parameter()
            item = la.read()
            if item != ',':
                la.rewind()
            return True
        elif item == '*':
            asterisk = 1
            item = la.read()
            la.rewind()
            if item != ',':
                parse_parameter()
            break
        else:
            la.rewind()
            parse_defparameter()
        item = la.read()
        if item == ending:
            la.rewind()
            return True
        elif item == ',':
            item = la.read()
            la.rewind()
            if item == ending:
                return True
    item = la.read()
    if item != ',':
        la.rewind()
        return True
    while True:
        item = la.read()
        if item == '**':
            parse_parameter()
            item = la.read()
            if item != ',':
                la.rewind()
            return True
        else:
            la.rewind()
            parse_defparameter()
            item = la.read()
            if asterisk:
                if item != ',':
                    la.rewind()
                    return True
            else:
                if item == ending:
                    la.rewind()
                    return True
                elif item == ',':
                    item = la.read()
                    la.rewind()
                    if item == ending:
                        return True

def parse_defparameter():
    parse_parameter()
    item = la.read()
    if item != '=':
        la.rewind()
    else:
        parse_expression()
    return True

def parse_parameter():
    item = la.read()
    if not is_id(item):
        la.syntax_error()
    item = la.read()
    if item == ':':
        parse_expression()
    else:
        la.rewind()
    return True

def parse_suite():
    item = la.read()
    if item != '':
        la.rewind()
        parse_stmt_list_plus_newline()
    else:
        item = la.read()
        if item == INDENT:
            while True:
                parse_statement()
                item = la.read()
                if item == DEDENT:
                    return True
                la.rewind()
        elif item == DEDENT:
            la.error('indentation_error: unindent does not match any outer indentation level')
        else:
            la.error('indentation_error: expected an indented block')

def parse_statement():
    item = la.read()
    la.rewind()
    if item == INDENT:
        la.error("indentation_error: unexpected indent")
    elif item in compound_statement_starts:
        parse_compound_statement()
    else:
        parse_stmt_list_plus_newline()

def parse_stmt_list_plus_newline():
    while True:
        parse_simple_stmt()
        item = la.read()
        if item == ';':
            item = la.read()
            if item == '':
                return True
            la.rewind()
        elif item == '':
            return True
        else:
            la.syntax_error()

#############################################################################
################################# EXECUTING #################################
#############################################################################

def test():
    if len(sys.argv) > 1:
        while True:
            item = la.read()
            if item == EOF:
                print la.items
                return True
            la.rewind()
            parse_statement()
    else:
        while True:
            item = la.read()
            la.rewind()
            if item in compound_statement_starts:
                la.in_block = 1
            parse_statement()
            print la.items
            if la.in_block:
                la.in_block = 0
                item = la.read()
                if item != '':
                    la.syntax_error()
            la.line_number = 0

if __name__ == '__main__':
    test()