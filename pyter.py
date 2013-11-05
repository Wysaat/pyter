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
    for item in items: print item

def strops(text):
    counter = 0
    for i in range(len(text)):
        if text[i] in str_ops:
            couter += 1
            if counter == 1:
                start = i
            elif counter == 2:
                end = i
                break
    str_item = ''.join

def main():
    while True:
        read('>>>')

if __name__ == '__main__':
    main()