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
               ',', '"', "'", "'''", '"""',
               '#,', '\\',]
operators = op_binary + op_unary + op_others

str_ops = ['"', "'", "'''", '"""']

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
    items = cut([text])
    for i in range(len(items)):
        if items[i] == '\\':
            if i != len(items) - 1:
                error("syntax_error: unexpected character after line continuation character")
            else:
                scan(state, "...")
        elif state == 0:
            if items[i] not in keywords + operators:
                state = 1 # a
        elif state == 1:
            if items[i] in op_binary:
                state = 2 # a *
        elif state == 2:
            if items[i] not in keywords + operators: # a * b
                state = 1
    print state
    print items

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
    items = cut_out_string(string)

    state = 0
    for index in range(len(items)):
        index += state
        if items[index][0] in str_ops:
            continue
        for op in op_binary_3:
            items[index] = items[index].replace(op, ' ' + op + ' ')
        splited = items[index].split()
        index_add = len(splited) - 1
        items = items[:index] + splited + items[index+1:]
        state += index_add
    state = 0
    for index in range(len(items)):
        index += state
        if items[index][0] in str_ops:
            continue
        if items[index] in op_binary_3:
            continue
        for op in op_binary_2:
            items[index] = items[index].replace(op, ' ' + op + ' ')
        splited = items[index].split()
        index_add = len(splited) - 1
        items = items[:index] + splited + items[index+1:]
        state += index_add
    state = 0
    for index in range(len(items)):
        index += state
        if items[index][0] in str_ops:
            continue
        if items[index] in op_binary_3 + op_binary_2:
            continue
        for op in op_binary_1 + op_unary + op_others:
            items[index] = items[index].replace(op, ' ' + op + ' ')
        splited = items[index].split()
        index_add = len(splited) - 1
        items = items[:index] + items[index].split() + items[index+1:]
        state += index_add
    return items

def cut(string):
    items = cut_out_string(string)
    new_items = []
    for item in items:
        if item[0] in ['"', "'"]:
            new_items.append(item)
        else:
            for op in op_binary_3:
                item = item.replace(op, ' '+op+' ')
                op_binary_3_parts = item.split()
                for part in op_binary_3_parts:
                    if part in op_binary_3:
                        op_binary_2_parts.append(part)
                    else:
                        for op in op_binary_2:
                            part = part.replace(op, ' '+op+' ')
                            op_binary_2_parts.append()

def cut(old_items):
    # old_items = cut_out_string(string)
    op_binary_list = [op_binary_1, op_binary_2, op_binary_3]
    escaped = []
    for i in range(2):
        items = []
        ops = op_binary_list.pop()
        for item in old_items:
            if item[0] in str_ops or item in escaped:
                items.append(item)
                print items
                print
            else:
                for op in ops:
                    item = item.replace(op, ' '+op+' ')
                    items.extend(item.split())
                    print items
                    print
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
        items = cut_out_string(string)
        print items

def test():
    cut(['a=', '"good"', '"day"', '+3**8/5-4--6', "you"])

if __name__ == '__main__':
    test()