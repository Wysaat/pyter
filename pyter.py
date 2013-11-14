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

op_binary_3 = ['**=',]

op_binary_2 = ['**', '==', '!=', '>=', '<=', '+=', '-=', '*=', '/=',]

op_binary_1 = ['+', '-', '*', '/',
               '=', '>', '<',
               '|', '&',
               ',']

op_binary = op_binary_3 + op_binary_2 + op_binary_1

op_unary    = ['~',]

op_others   = ['(', ')', '[', ']', '{', '}',
               ';', ':', '#', '\\',]

str_ops = ['"', "'", "'''", '"""']

operators = op_binary + op_unary + op_others + str_ops

reserved = keywords + operators

def cut_out_string(string):
    items = []

    def find_start(string):
        for i in range(len(string)):
            if string[i] in str_ops:
                if string[i:i+3] in str_ops:
                    return i, string[i:i+3]
                else:
                    return i, string[i]
        return -1, ''

    def find_end(string, op):
        for i in range(len(string)-1):
            if string[i] == '\\':
                tor = string[i:i+2]
                string = string.replace(tor, 'XX')
        if op in string:
            return string.find(op)
        return -1

    while True:
        start, op = find_start(string)
        if start < 0:
            return items + [string]
        if len(string[:start]) > 0:
            items.append(string[:start])
        string = string[start:]
        end = find_end(string[len(op):], op)
        # not the real "end": end of string[1:]
        while end < 0:
            if op in ['"""', "'''"]:
                print "...",
                string += '\n' + raw_input()
                end = find_end(string[len(op):], op)
            elif string.endswith('\\'):
                print "...",
                string = string.rstrip('\\')
                string += raw_input()
                end = find_end(string[len(op):], op)
            else:
                error("syntax_error: EOL while scanning string literal")
        # get the real "end"
        end += len(op)
        items.append(string[:end+len(op)])
        string = string[end+len(op):]
        if string == '':
            return items

def error(string):
    print string
    exit()

def syntax_error():
    error('syntax_error: invalid syntax')

def cut(string):
    old_items = cut_out_string(string)
    op_list = [op_binary_1 + op_unary + op_others, op_binary_2, op_binary_3]
    escaped = []
    for i in range(3):
        items = []
        ops = op_list.pop()
        for item in old_items:
            if item[0] in str_ops or item in escaped:
                items.append(item)
            else:
                for op in ops:
                    item = item.replace(op, ' '+op+' ')
                items.extend(item.split())
        old_items = items
        escaped += ops
    return old_items

def scan(items):
    expect = []
    undesired = []
    end = 1
    def new_expect(got):
        global expect, undesired, end
        if got not in reserved:
            expect = op_binary
        elif got in op_binary:
            end = 0
            undesired = op_binary + op_others + keywords
        elif got in ['[', '(',]:
            undesired = op_binary + keywords

    index = 0
    while True:
        if index == len(items) - 1:
            if end == 1:
                execute(items)
            else:
                error('syntax_error: invalid syntax')
        else:
            got = items[index]
            index += 1
            if got not in expect:
                if got not in undisired:
                    new_expect(got)
                else:
                    error('syntax_error: invalid syntax')

def scan_noun(items):
    if items[0] not in keywords + operators:
        return items[1:]
    elif items[0] is '[':
        return scan_square_bracket(items[1:])
    elif items[0] is '(':
        return scan_bracket(items[1:])
    elif items[0] in op_unary:
        return scan_noun(items[1:])
    else:
        syntax_error()

def scan_square_bracket(items):
    items = scan_noun(items)
    if len(items) == 0:
        syntax_error()
    elif items[0] is ']':
        return items[1:]
    else:
        while True:
            items = scan_noun(items)
            if items[0] in op_binary:
                if len(items[1:]) == 0:
                    syntax_error()
                elif items[1] is ']':
                    if items[0] is ',':
                        return items[2:]
                    else:
                        syntax_error()
                items = items[1:]
            elif items[0] is ']':
                return items[1:]
            # haven't implemented [i for i in range(4)]...
            else:
                syntax_error()

def main():
    while True:
        read()

def test():
    while True:
        print ">>>",
        string = raw_input()
        print cut(string)

if __name__ == '__main__':
    test()