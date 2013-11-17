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