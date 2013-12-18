while True:
    start, op = find_start(string)
    if start < 0:
        return items + [string]
    if len(string[:start]) > 0:
        items.append(string[:start])
    string = string[start:]
    end = find_end(string[len(op):], op)
    # not the real "end": end of string[1:]
    while end < 0: ?
        if op in ['"""', "'''"]:
            print("...")
            string += '\n' + raw_input()
            end = find_end(string[len(op):], op)
        elif string.endswith('\\'):
            print ("...")
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