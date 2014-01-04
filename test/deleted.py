def parse_list_for(ending):
    item = la.read()
    if item != 'for':
        la.syntax_error()
    parse_target_list('in')
    item = la.read()
    if item != 'in':
        la.syntax_error()
    parse_expression_nocond()
    item = la.read()
    if item == ',':
        while True:
            parse_expression_nocond()
            item = la.read()
            if item != ',':
                break
            item = la.read()
            if item in ['for', 'if', ending]:
                break
            la.rewind()
    la.rewind()
    if item == 'for':
        parse_list_for(ending)
    elif item == 'if':
        parse_list_if(ending)
    else:
        return True

def parse_list_if(ending):
    item = la.read()
    if item != 'if':
        la.syntax_error()
    parse_expression_nocond()
    item = la.read()
    la.rewind()
    if item == 'for':
        parse_list_for(ending)
    elif item == 'if':
        parse_list_if(ending)
    else:
        return True
