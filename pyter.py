from __future__ import division
import operator
import math
import copy

env = vars(math)
env.update(vars(operator))

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
               '|', '&']

op_binary = op_binary_3 + op_binary_2 + op_binary_1

op_unary    = ['~',]

op_others   = ['(', ')', '[', ']', '{', '}',
               ',', ';', ':', '#', '\\',]

str_ops = ['"', "'", "'''", '"""']

operators = op_binary + op_unary + op_others + str_ops

def read(ind='>>>', ind2='...'):
    print ind,
    text = raw_input()
    items = cut([text])
    while text.endswith("\\"):
        print ind2,
        text = text[:-1]
        text += raw_input()
    # if text.endswith(":"):
    items = cut([text])
    while "[" in items and "]" not in items:
        print ind2,
    # if items == ['exit', '(', ')']:
    
    #     exit()
    # print items

'''
    state numbering:
    a = 3 + 2
    a = 3 + 2 * 6
    a = (3 + 2) * 6
'''
def scan(state=0, ind=">>>"):
    print ind,
    text = raw_input()
    items = cut(text)
    for i in range(len(items)):
        if items[i] == '\\':
            if i != len(items) - 1:
                error("syntax_error: unexpected character after line continuation character")
            else:
                scan(state, "...")
        elif state == 0: # nothing now, willing to get anything
            if items[i] not in keywords + operators:
                state = 1 # a
            elif items[i] in ['(', '[',]:
                open_op = items[i]
                state = 3 # (
        elif state == 1:
            if items[i] in op_binary:
                state = 2 # a *
        elif state == 2:
            if items[i] not in keywords + operators: # a * b
                state = 1
        elif state == 3:
            if items[i] not in keywords + operators:
                state = 4 # (a
        elif state == 4:
            if items[i] in op_binary:
                state = 5 # (a *
            elif items[i] in [',',]:
                state = 3
            elif items[i] == open_op:
                del open_op
                state = 1
        elif state == 5:
            if items[i] not in keywords + operators:
                state = 3
            elif items[i] in ['(', '[',]:
                state = endless_states_to_enumerate
    print state
    print items

def scan(items):
    if items[0] not in keywords + operators:
        items = scan_noun(items)
        state = 'noun'
    elif items[0] in op_unary:
        items = scan_noun(items[1:])
        state = 'noun'

def expect(got):
    expect = []
    if got == 'noun':
        expect.append('nothing')
        expect.append('op_binary')
    elif got == 'op_binary':
        expect.append('noun')

def scan(items, expect):
    got = get_from(items)
    if got in expect:
        return expect(got)
    else:
        error('syntax_error: invalid syntax')

def scan_noun(items):
    index = 0
    while True:
        if items[index] not in keywords + operators:
            if items[index+1]

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

def main():
    while True:
        read()

def test():
    while True:
        print ">>>",
        string = raw_input()
        print cut(string)

# def test():
#     cut(['a=', '"good"', '"day"', '+3**8/5-4--6', "you"])

if __name__ == '__main__':
    test()