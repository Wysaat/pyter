#include "cpyter.h"

void rollback(scanner *sc) {
    sc->rbf = 1;
}

char *sc_read_num_lit(scanner *sc, buffer *buff) {
    if (sc_curch(sc) == '.') {
        buff_add(buff, sc_readch(sc));
        while ((sc_curch(sc) >= '0' && sc_curch(sc) <= '9'))
            buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'j')
            buff_add(buff, sc_readch(sc));
        return buff_puts(buff);
    }
    while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
        buff_add(buff, sc_readch(sc));
    if (sc_curch(sc) == '.') {
        buff_add(buff, sc_readch(sc));
        while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
            buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'j')
            buff_add(buff, sc_readch(sc));
        return buff_puts(buff);
    }
    else if (sc_curch(sc) == 'j') {
        buff_add(buff, sc_readch(sc));
        return buff_puts(buff);
    }
    else
        return buff_puts(buff);
}

char *sc_read_str_lit(scanner *sc, buffer *buff) {
    int i = 1;
    char op = sc_readch(sc), ch;
    buff_add(buff, op);
    while ((ch = sc_readch(sc)) != op)
        buff_add(buff, ch);
    buff_add(buff, op);
    return buff_puts(buff);
}

char *sc_rread(scanner *sc, buffer *buff) { /* raw read */
    int i, j, k = 0;
    char *token;
    if (!sc->eolf) {
        if (sc_curch(sc) == '\n') {
            sc->eolf = 1;
            return strdup("\n");
        }
        while (sc_curch(sc) == ' ' || sc_curch(sc) == '\t')
            sc->ind++;
        for (j = 3; j > 0; j--)
            if (is_spctk(sc_curchs(sc, j)))
                return sc_readchs(sc, j);
        if (sc_curch(sc) == '.') {
            if (sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9')
                return sc_read_num_lit(sc, buff);
            buff_add(buff, sc_curch(sc));
            return buff_puts(buff);
        }
        if (sc_curch(sc) >= '0' && sc_curch(sc) <= '9') {
            return sc_read_num_lit(sc, buff);
        }
        if (sc_curch(sc) == '"' || sc_curch(sc) == '\'')
            return sc_read_str_lit(sc, buff);
        while (is_alphnum(sc_curch(sc)) || sc_curch(sc) == '_')
            buff_add(buff, sc_readch(sc));
        char *tk = buff_puts(buff);
        if (is_strprfx(tk) && (sc_curch(sc) == '\'' || sc_curch(sc) == '"'))
            return stradd(tk, sc_read_str_lit(sc, buff));
        else
            return tk;
    }
    else {
        puts("in sc_rread end, NOT IMPLEMENTING EOL");
        exit(0);
    }
}

char *sc_read(scanner *sc) {
    if (sc->rbf) {
        sc->rbf = 0; /* clear rollback flag */
        return sc->lasttk;
    }
    buffer *buff = buff_init();
    char *retptr = sc_rread(sc, buff);
    buff_del(buff);
    sc->lasttk = retptr;
    return retptr;
}

int is_num(char ch) {
    return ch >= '0' && ch <= '9';
}


int is_alph(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int is_alphnum(char ch) {
    return is_alph(ch) || is_num(ch);
}

void *parse_atom(scanner *sc) {
    char *token = sc_read(sc);
    if (is_identifier(token))
        return IDENTIFIER(token);
    if (is_int(token))
        return INT_EXPR(token);
    else if (is_float(token))
        return FLOAT_EXPR(token);
    else if (is_imag(token))
        return IMAG_EXPR(token);
    else if (is_str(token))
        return STR_EXPR(token);
    else if (!strcmp(token, "(")) {
        list *expr_head = list_node();
        token = sc_read(sc);
        if (!strcmp(token, ")"))
            return PARENTH_FORM(expr_head);
        rollback(sc);
        void *expression = parse_expression(sc);
        token = sc_read(sc);
        if (!strcmp(token, ")"))
            return expression;
        else if(!strcmp(token, ",")) {
            list_append_content(expr_head, expression);
            token = sc_read(sc);
            if (!strcmp(token, ")"))
                return PARENTH_FORM(expr_head);
            rollback(sc);
            list_append_list(expr_head, pa_exprs(sc, ")"));
            void *retptr = PARENTH_FORM(expr_head);
            token = sc_read(sc);
            if (!strcmp(token, ")"))
                return retptr;
        }
    }
    else if (!strcmp(token, "[")) {
        list *expr_head = list_node();
        token = sc_read(sc);
        if (!strcmp(token, "]"))
            return LIST_EXPR(expr_head);
        rollback(sc);
        void *expression = parse_expression(sc);
        list_append_content(expr_head, expression);
        token = sc_read(sc);
        if (!strcmp(token, "]"))
            return LIST_EXPR(expr_head);
        else if (!strcmp(token, ",")) {
            token = sc_read(sc);
            if (!strcmp(token, "]"))
                return LIST_EXPR(expr_head);
            rollback(sc);
            list_append_list(expr_head, pa_exprs(sc, "]"));
            token = sc_read(sc);
            if (!strcmp(token, "]"))
                return LIST_EXPR(expr_head);
        }
    }
    else if (!strcmp(token, "{")) {
        list *expr_head = list_node();
        token = sc_read(sc);
        if (!strcmp(token, "}"))
            return SET_EXPR(expr_head);
        rollback(sc);
        void *expression = parse_expression(sc);
        list_append_content(expr_head, expression);
        token = sc_read(sc);
        if (!strcmp(token, "}"))
            return SET_EXPR(expr_head);
        else if (!strcmp(token, ",")) {
            token = sc_read(sc);
            if (!strcmp(token, "}"))
                return SET_EXPR(expr_head);
            rollback(sc);
            list_append_list(expr_head, pa_exprs(sc, "}"));
            token = sc_read(sc);
            if (!strcmp(token, "}"))
                return SET_EXPR(expr_head);
        }
        else if (!strcmp(token, ":")) {
            list *expr_head2 = list_node();
            list_append_content(expr_head2, parse_expression(sc));
            token = sc_read(sc);
            if (!strcmp(token, "}"))
                return DICT_EXPR(expr_head, expr_head2);
            else if (!strcmp(token, ",")) {
                token = sc_read(sc);
                if (!strcmp(token, "}"))
                    return DICT_EXPR(expr_head, expr_head2);
                rollback(sc);
                list **lists = pa_dict_items(sc);
                list_append_list(expr_head, lists[0]);
                list_append_list(expr_head2, lists[1]);
                token = sc_read(sc);
                if (!strcmp(token, "}"))
                    return DICT_EXPR(expr_head, expr_head2);
            }
        }
    }
}

void *parse_primary(scanner *sc) {
    char *token;
    void *primary = parse_atom(sc);
    while (1) {
        token = sc_read(sc);
        if (!strcmp(token, "[")) {
            void *val = pa_sll_or_subs(sc);
            token = sc_read(sc);
            if (!strcmp(token, "]")) {
                if (type(val) == slice_expr_t)
                    primary = SLICING(primary, (slice_expr *)val);
                else if (type(val) == subsc_expr_t)
                    primary = SUBSCRIPTION(primary, (subsc_expr *)val);
            }
        }
        else {
            rollback(sc);
            return primary;
        }
    }
}

void *parse_power(scanner *sc) {
    void *primary = parse_primary(sc);
    char *token = sc_read(sc);
    if (strcmp(token, "**")) {
        rollback(sc);
        return primary;
    }
    return POWER(primary, parse_u_expr(sc));
}

void *parse_u_expr(scanner *sc) {
    char *token = sc_read(sc);
    if (!strcmp(token, "+") || !strcmp(token, "-") || !strcmp(token, "~"))
        return U_EXPR(token, parse_u_expr(sc));
    rollback(sc);
    return parse_power(sc);
}

void *parse_m_expr(scanner *sc) {
    char *token;
    void *expr = parse_u_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!(!strcmp(token, "*") || !strcmp(token, "//") ||
              !strcmp(token, "/") || !strcmp(token, "%"))) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_u_expr(sc));
    }
}

void *parse_a_expr(scanner *sc) {
    char *token;
    void *expr = parse_m_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!(!strcmp(token, "+") || !strcmp(token, "-"))) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_m_expr(sc));
    }
}

void *parse_shift_expr(scanner *sc) {
    char *token;
    void *expr = parse_a_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!!strcmp(token, "<<") || !strcmp(token, ">>")) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_a_expr(sc));
    }
}

void *parse_and_expr(scanner *sc) {
    char *token;
    void *expr = parse_shift_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!!strcmp(token, "&")) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_shift_expr(sc));
    }
}

void *parse_xor_expr(scanner *sc) {
    char *token;
    void *expr = parse_and_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!!strcmp(token, "^")) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_and_expr(sc));
    }
}

void *parse_or_expr(scanner *sc) {
    char *token;
    void *expr = parse_xor_expr(sc);
    while (1) {
        token = sc_read(sc);
        if (!!strcmp(token, "|")) {
            rollback(sc);
            return expr;
        }
        expr = B_EXPR(expr, token, parse_xor_expr(sc));
    }
}

int in_cmpop(char *token) {
    if (!strcmp(token, "is") || !strcmp(token, "not") || !strcmp(token, "in"))
        return 1;
    return is_cmpop(token);
}

void *parse_comparison(scanner *sc) {
    char *token;
    char *op;
    void *or_expr = parse_or_expr(sc);
    token = sc_read(sc);
    rollback(sc);
    if (!in_cmpop(token))
        return or_expr;
    void *left = or_expr;
    void *right;
    list *comparisons = list_node();
    comparison *comp_expr = (comparison *)malloc(sizeof(comparison));
    comp_expr->type = comparison_t;
    while (1) {
        token = sc_read(sc);
        if (!strcmp(token, "is")) {
            token = sc_read(sc);
            if (!strcmp(token, "not"))
                op = "is not";
            else {
                rollback(sc);
                op = "is";
            }
        }
        else if (!strcmp(token, "not")) {
            token = sc_read(sc);
            if (!strcmp(token, "in"))
                op = "not in";
        }
        else if (!strcmp(token, "in"))
            op = "in";
        else if (in_cmpop(token))
            op = token;
        else {
            rollback(sc);
            comp_expr->comparisons = comparisons;
            return comp_expr;
        }
        right = parse_or_expr(sc);
        list_append_content(comparisons, B_EXPR(left, op, right));
        left = right;
    }
}

void *parse_not_test(scanner *sc) {
    char *token = sc_read(sc);
    if (!strcmp(token, "not"))
        return NOT_TEST(parse_not_test(sc));
    rollback(sc);
    return parse_comparison(sc);
}

void *parse_and_test(scanner *sc) {
    char *token;
    void *test = parse_not_test(sc);
    while (1) {
        token = sc_read(sc);
        if (strcmp(token, "and")) {
            rollback(sc);
            return test;
        }
        test = B_EXPR(test, token, parse_not_test(sc));
    }
}

void *parse_or_test(scanner *sc) {
    char *token;
    void *test = parse_and_test(sc);
    while (1) {
        token = sc_read(sc);
        if (strcmp(token, "or")) {
            rollback(sc);
            return test;
        }
        test = B_EXPR(test, token, parse_and_test(sc));
    }
}

void *parse_conditional_expression(scanner *sc) {
    void *or_test = parse_or_test(sc);
    char *token = sc_read(sc);
    if (strcmp(token, "if")) {
        rollback(sc);
        return or_test;
    }
    void *or_test2 = parse_or_test(sc);
    token = sc_read(sc);
    if (!strcmp(token, "else"))
        return CONDITIONAL_EXPRESSION(or_test, or_test2, parse_expression(sc));
}

/* no lambda expression support */
void *parse_expression(scanner *sc) {
    return parse_conditional_expression(sc);
}

/* no lambda expression support */
void *parse_expression_nocond(scanner *sc) {
    return parse_or_test(sc);
}

list *pa_exprs(scanner *sc, char *ending) {
    char *token;
    list *expr_head = list_node();
    while (1) {
        list_append_content(expr_head, parse_expression(sc));
        token = sc_read(sc);
        if (!strcmp(token, ",")) {
            token = sc_read(sc);
            rollback(sc);
            if (!strcmp(token, ending))
                return expr_head;
        }
        else {
            rollback(sc);
            return expr_head;
        }
    }
}

list **pa_dict_items(scanner *sc) {
    char *token;
    list **retptr = (list **)malloc(sizeof(list *));
    list *expr_head = list_node();
    list *expr_head2 = list_node();
    retptr[0] = expr_head;
    retptr[1] = expr_head2;

    while (1) {
        list_append_content(expr_head, parse_expression(sc));
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            list_append_content(expr_head2, parse_expression(sc));
        }
        token = sc_read(sc);
        if (!strcmp(token, ",")) {
            token = sc_read(sc);
            rollback(sc);
            if (!strcmp(token, "}"))
                return retptr;
        }
        else {
            rollback(sc);
            return retptr;
        }
    }
}

void *pa_sll_or_subs(scanner *sc) {
    char *token;
    void *start, *stop, *step;

    start = parse_expression(sc);
    token = sc_read(sc);
    if (!strcmp(token, ":")) {
        stop = parse_expression(sc);
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            step = parse_expression(sc);
            return SLICE_EXPR(start, stop, step);
        }
    }
    else {
        rollback(sc);
        return SUBSC_EXPR(start);
    }
}

void *parse_expression_list(scanner *sc, char *ending) {
    void *expression = parse_expression(sc);
    char *token = sc_read(sc);
    if (!strcmp(token, ",")) {
        list *expressions = list_node();
        list_append_content(expressions, expression);
        token = sc_read(sc);
        rollback(sc);
        if (!strcmp(token, ending))
            return EXPRESSION_LIST(expressions);
        list_append_list(expressions, pa_exprs(sc, ending));
        return EXPRESSION_LIST(expressions);
    }
    else {
        rollback(sc);
        return expression;
    }
}

// void *parse_simple_stmt(scanner *sc) {
//     void *expression_list1 = parse_expression_list(sc, ";");
//     char *token = sc_read(sc);
//     if (!strcmp(token, "=")) {
//         void *expression_list2 = parse_expression_list(sc, ";");
//         return assignment_stmt(expression_list1, expression_list2);
//     }
//     else {
//         rollback(sc);
//         return expression_list1;
//     }
// }

int test1()
{
    scanner *sc = sc_init();
    sc_getline(sc, stdin);
    void *expr = parse_expression(sc);
    void *retval = evaluate(expr);
    // if (*(int *)retval == pylist_t)
    //     pylist__sort__((pylist *)retval, pyint__cmp__);
    print(retval);
    return 0;
}

int test2()
{
    // environment *global_env = environment_init();
    scanner *sc = sc_init();
    sc_getline(sc, stdin);
    void *exprs = parse_expression_list(sc, ";");
    void *vals = evaluate(exprs);
    print(vals);
    return 0;
}

void interpret()
{
    scanner *sc = sc_init();
    while (1) {
        sc_getline(sc, stdin);
        void *exprs = parse_expression_list(sc, ";");
        print(evaluate(exprs));
    }
}

int main()
{
    interpret();
    return 0;
}