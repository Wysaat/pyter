from __future__ import division
import operator
import math
import copy

env = vars(math)
env.update(vars(operator))

keywords = ['def',
            'class',
            'while',
            'import',]

operators = ['+', '-', '*', '/', '**',
             '=', '>', '<', '==', '!=',
            '(', ')', '[', ']', '{', '}',
            ',', '"', "'", "'''",
            '~', '|', '&',
            '#,', '\\',]
str_ops = ['"', "'", "'''"]

def read(ind):
    print ind,
    text = raw_input()
    for op in operators:
        text = text.replace(op, ' ' + op + ' ')
    items = text.split()

    if items == ['exit', '(', ')']:
        exit()
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
        return items

def cut(items):
    items = cut_out_string(items)
    for index in range(len(items)):
        if items[index][0] == items[index][-1] and \
            items[index][0] in str_ops:
            continue
        for op in operators:
            items[index] = items[index].replace(op, ' ' + op + ' ')
        items = items[:index] + items[index].split() + items[index+1:]
    return items

def main():
    while True:
        read('>>>')

def test():
    print cut_out_string(["a = 'helloword'"])

if __name__ == '__main__':
    test()