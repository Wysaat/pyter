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

# def cut_out_string(items, line=0):
#     for index in range(len(items)):
#         if items[index][0] in str_ops and \
#             items[index][0] == items[index][-1]:
#             continue
#         counter = 0
#         for i in range(len(items[index])):
#             if items[index][i] in str_ops:
#                 the_op = items[index][i]
#                 counter += 1
#                 if counter == 1:
#                     start = i
#                 elif counter == 2:
#                     if items[index][i] == the_op:
#                         end = i
#                         head = items[index][:start]
#                         body = items[index][start:end+1]
#                         tail = items[index][end+1:]
#                         items = items[:index] + [head] + [body] + [tail] + items[index+1:]
#                         break
#                     else:
#                         counter -= 1
#         if counter == 1 and items[index][-1] != '\\':
#             print "syntax_error: EOL while scanning string literal"
#             scan()
#         elif counter == 1 and items[index][-1] == '\\':
#             head = items[index][:start]
#             rest = items[index][start:]
#             items = items[:index] + [head] + [rest] + items[index+1:]

#     print 'in cut_out_string:', items
#     return [item for item in items if item != '']

def cut_out_string(string):
    str_ops = ['"', "'"]
    items = []
    def find(string, str_ops):
        inds = []
        for op in str_ops:
            ind = string.find(op)
            if ind >= 0:
                inds.append(ind)
        if len(inds) > 0:
            return min(inds)
        else:
            return -1
    while True:
        start = find(string, str_ops)
        if start < 0:
            return items + [string]
        op = string[start]
        if len(string[:start]) > 0:
            items.append(string[:start])
        string = string[start:]
        # not the real "end": end of string[1:]
        end = string[1:].find(op)
        while end < 0:
            if string.endswith('\\'):
                print "...",
                string = string.rstrip('\\')
                string += raw_input()
                end = string[1:].find(op)
            else:
                error("syntax_error: EOL while scanning string literal")
        # get the real "end"
        end += 1
        items.append(string[:end+1])
        string = string[end+1:]
        if string == '':
            return items

def error(string):
    print string
    exit()

def cut(items):
    last_items = []
    while items != last_items:
        last_items = items
        items = cut_out_string(items)
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

def main():
    while True:
        read()

def test():
    while True:
        print ">>>",
        string = raw_input()
        items = cut_out_string(string)
        print items

if __name__ == '__main__':
    test()