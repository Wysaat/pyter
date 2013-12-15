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

