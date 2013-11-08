from __future__ import division
import operator
import math
import copy

env = vars(math)
env.update(vars(operator))

keywords = ['def',
            'class',
            'while',
            'for',
            'in',
            'range',
            'import',
            'from',]

op_binary_1 = ['==', '**', '==', '!=', '>=', '<=', '+=', '-=', '*=', '/=', '**=',]

op_binary_2 = ['+', '-', '*', '/',
               '=', '>', '<',
               '|', '&']

op_binary = op_binary_1 + op_binary_2

op_unary    = ['~',]

op_others   = ['(', ')', '[', ']', '{', '}',
               ',', '"', "'", "'''", '"""',
               '#,', '\\',]
operators = op_binary_1 + op_binary_2 + op_unary + op_others

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

def cut_out_string(items):
    for index in range(len(items)):
        if items[index][0] == items[index][-1] and \
            items[index][0] in str_ops:
            continue
        counter = 0
        for i in range(len(items[index])):
            if items[index][i] in str_ops:
                the_op = items[index][i]
                counter += 1
                if counter == 1:
                    start = i
                elif counter == 2:
                    if items[index][i] == the_op:
                        end = i
                        head = items[index][:start]
                        body = items[index][start:end+1]
                        tail = items[index][end+1:]
                        items = items[:index] + [head] + [body] + [tail] + items[index+1:]
                        break
                    else:
                        counter -= 1
        if i == (len(items[index]) - 1) and counter == 1:
            print "syntax_error: EOL while scanning string literal"
            main()
    return [item for item in items if item != '']

def cut(items):
    last_items = []
    while items != last_items:
        last_items = items
        items = cut_out_string(items)
    state = 0
    for index in range(len(items)):
        index += state
        if items[index][0] == items[index][-1] and \
            items[index][0] in str_ops:
            continue
        for op in op_binary_1:
            items[index] = items[index].replace(op, ' ' + op + ' ')
        splited = items[index].split()
        index_add = len(splited) - 1
        items = items[:index] + splited + items[index+1:]
        state += index_add
    state = 0
    for index in range(len(items)):
        index += state
        if items[index][0] == items[index][-1] and \
            items[index][0] in str_ops:
            continue
        if items[index] in op_binary_1:
            continue
        for op in op_binary_2 + op_unary + op_others:
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
    assert cut(['a = "hello, world!" + "again!"']) == ['a', '=', '"hello, world!"', '+', '"again!"']
    print cut(['a = 3 ** 2 * 4'])
    print cut(['a = 3 *** 2'])
    print "All tests passed..."

if __name__ == '__main__':
    while True:
        scan()