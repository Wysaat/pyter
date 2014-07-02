#include "cpyter.h"

void rollback(scanner *sc) {
    sc->rbf = 1;
}

char *sc_read_num_lit(scanner *sc, buffer *buff) {
    if (sc_curch(sc) == '0') {
        buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'b' || sc_curch(sc) == 'B') {
            buff_add(buff, sc_readch(sc));
            while (sc_curch(sc) == '0' || sc_curch(sc) == '1')
                buff_add(buff, sc_readch(sc));
            return buff_puts(buff);
        }
        else if (sc_curch(sc) == 'o' || sc_curch(sc) == 'O') {
            buff_add(buff, sc_readch(sc));
            while (sc_curch(sc) >= '0' && sc_curch(sc) <= '7')
                buff_add(buff, sc_readch(sc));
            return buff_puts(buff);
        }
        else if (sc_curch(sc) == 'x' || sc_curch(sc) == 'X') {
            buff_add(buff, sc_readch(sc));
            while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9' ||
                   sc_curch(sc) >= 'a' && sc_curch(sc) <= 'f' ||
                   sc_curch(sc) >= 'A' && sc_curch(sc) <= 'F')
                buff_add(buff, sc_readch(sc));
            return buff_puts(buff);
        }
    }
    if (sc_curch(sc) == '.') {
        buff_add(buff, sc_readch(sc));
        while ((sc_curch(sc) >= '0' && sc_curch(sc) <= '9'))
            buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J')
            buff_add(buff, sc_readch(sc));
        else if (sc_curch(sc) == 'e' || sc_curch(sc) == 'E') {
            buff_add(buff, sc_readch(sc));
            if (sc_curch(sc) == '+' || sc_curch(sc) == '-')
                buff_add(buff, sc_readch(sc));
            while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
                buff_add(buff, sc_readch(sc));
            if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J')
                buff_add(buff, sc_readch(sc));
        }
        return buff_puts(buff);
    }
    while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
        buff_add(buff, sc_readch(sc));
    if (sc_curch(sc) == '.') {
        buff_add(buff, sc_readch(sc));
        while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
            buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J')
            buff_add(buff, sc_readch(sc));
        else if (sc_curch(sc) == 'e' || sc_curch(sc) == 'E') {
            buff_add(buff, sc_readch(sc));
            if (sc_curch(sc) == '+' || sc_curch(sc) == '-')
                buff_add(buff, sc_readch(sc));
            while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
                buff_add(buff, sc_readch(sc));
            if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J')
                buff_add(buff, sc_readch(sc));
        }
        return buff_puts(buff);
    }
    else if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J') {
        buff_add(buff, sc_readch(sc));
        return buff_puts(buff);
    }
    else if (sc_curch(sc) == 'e' || sc_curch(sc) == 'E') {
        buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == '+' || sc_curch(sc) == '-')
            buff_add(buff, sc_readch(sc));
        while (sc_curch(sc) >= '0' && sc_curch(sc) <= '9')
            buff_add(buff, sc_readch(sc));
        if (sc_curch(sc) == 'j' || sc_curch(sc) == 'J')
            buff_add(buff, sc_readch(sc));
        return buff_puts(buff);
    }
    else
        return buff_puts(buff);
}

char *sc_read_str_lit(scanner *sc, buffer *buff) {
    buff_add(buff, '\'');
    char ch;
    while (1) {
        while (sc_skip(sc, sc_curch(sc)))
            sc_readch(sc);
        if (!sc_curch(sc)) {
            sc_getline(sc);
        }
        else if (sc_curch(sc) == '\'' || sc_curch(sc) == '"') {
            char op = sc_readch(sc);
            if (sc_curch(sc) == op && sc_nxtch(sc) == op) {
                sc->skip_newlines++;
                sc_readch(sc);
                sc_readch(sc);
                int opnum = 0;
                int _skip = 0;
                while (1) {
                    ch = sc_readch(sc);
                    if (!_skip && ch == '\\')
                        _skip = 1;
                    else if (_skip) {
                        _skip = 0;
                        if (ch == '"' || ch == '\'' || ch == '\\')
                            buff_add(buff, ch);
                        else if (ch == 'a')
                            buff_add(buff, '\a');
                        else if (ch == 'b')
                            buff_add(buff, '\b');
                        else if (ch == 'f')
                            buff_add(buff, '\f');
                        else if (ch == 'n')
                            buff_add(buff, '\n');
                        else if (ch == 'r')
                            buff_add(buff, '\r');
                        else if (ch == 't')
                            buff_add(buff, '\t');
                        else if (ch == 'v')
                            buff_add(buff, '\v');
                        else if (ch >= '0' && ch < '8') {
                            int val = ch - '0';
                            if (sc_nxtch(sc) >= '0' && sc_nxtch(sc) < '8') {
                                ch = sc_readch(sc);
                                val *= 8;
                                val += ch;
                                if (sc_nxtch(sc) >= '8' && sc_nxtch(sc) < '8') {
                                    ch = sc_readch(sc);
                                    val *= 8;
                                    val += ch;
                                    buff_add(buff, (char)val);
                                }
                                else
                                    buff_add(buff, (char)val);
                            }
                            else
                                buff_add(buff, (char)val);
                        }
                        else if (ch == 'x') {
                            int val = 0;
                            if ((sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9') ||
                                (sc_nxtch(sc) >= 'a' && sc_nxtch(sc) <= 'f') ||
                                (sc_nxtch(sc) >= 'A' && sc_nxtch(sc) <= 'F')) {
                                ch = sc_readch(sc);
                                val *= 16;
                                val += ch;
                                if ((sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9') ||
                                    (sc_nxtch(sc) >= 'a' && sc_nxtch(sc) <= 'f') ||
                                    (sc_nxtch(sc) >= 'A' && sc_nxtch(sc) <= 'F')) {
                                    ch = sc_readch(sc);
                                    val *= 16;
                                    val += ch;
                                    buff_add(buff, (char)val);
                                }
                            }
                        }
                    }
                    else if (ch == op) {
                        opnum++;
                        if (opnum == 3) {
                            sc->skip_newlines--;
                            break;
                        }
                    }
                    else {
                        while (opnum--)
                            buff_add(buff, op);
                        opnum = 0;
                        if (!ch) {
                            sc->ps = sc->ps2;
                            sc_getline(sc);
                        }
                        else
                            buff_add(buff, ch);
                    }
                }
            }
            else {
                int _skip = 0;
                while (1) {
                    ch = sc_readch(sc);
                    if (!_skip && ch == '\\')
                        _skip = 1;
                    else if (_skip) {
                        _skip = 0;
                        if (ch == '"' || ch == '\'' || ch == '\\')
                            buff_add(buff, ch);
                        else if (ch == 'a')
                            buff_add(buff, '\a');
                        else if (ch == 'b')
                            buff_add(buff, '\b');
                        else if (ch == 'f')
                            buff_add(buff, '\f');
                        else if (ch == 'n')
                            buff_add(buff, '\n');
                        else if (ch == 'r')
                            buff_add(buff, '\r');
                        else if (ch == 't')
                            buff_add(buff, '\t');
                        else if (ch == 'v')
                            buff_add(buff, '\v');
                        else if (ch >= '0' && ch < '8') {
                            int val = ch - '0';
                            if (sc_nxtch(sc) >= '0' && sc_nxtch(sc) < '8') {
                                ch = sc_readch(sc);
                                val *= 8;
                                val += ch;
                                if (sc_nxtch(sc) >= '8' && sc_nxtch(sc) < '8') {
                                    ch = sc_readch(sc);
                                    val *= 8;
                                    val += ch;
                                    buff_add(buff, (char)val);
                                }
                                else
                                    buff_add(buff, (char)val);
                            }
                            else
                                buff_add(buff, (char)val);
                        }
                        else if (ch == 'x') {
                            int val = 0;
                            if ((sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9') ||
                                (sc_nxtch(sc) >= 'a' && sc_nxtch(sc) <= 'f') ||
                                (sc_nxtch(sc) >= 'A' && sc_nxtch(sc) <= 'F')) {
                                ch = sc_readch(sc);
                                val *= 16;
                                val += ch;
                                if ((sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9') ||
                                    (sc_nxtch(sc) >= 'a' && sc_nxtch(sc) <= 'f') ||
                                    (sc_nxtch(sc) >= 'A' && sc_nxtch(sc) <= 'F')) {
                                    ch = sc_readch(sc);
                                    val *= 16;
                                    val += ch;
                                    buff_add(buff, (char)val);
                                }
                            }
                        }
                    }
                    else if (ch == op)
                        break;
                    else
                        buff_add(buff, ch);
                }
            }
        }
        else {
            buff_add(buff, '\'');
            sc->ps = sc->ps1;
            return buff_puts(buff);
        }
    }
}

char *sc_rread(scanner *sc, buffer *buff) { /* raw read */
    int i, j, k = 0;
    char *token;

    while (1) {
        while (sc_skip(sc, sc_curch(sc)))
            sc->ind++;
        if (sc_curch(sc) == 0) {
            sc_getline(sc);
            sc->indentf = 0;
            sc->dedentf = 0;
            if (sc->eoff)
                return 0;
        }
        else
            break;
    }

    if (sc_curch(sc) == '\n') {
        sc->eolf = 1;
        return strdup("\n");
    }
    for (j = 3; j > 0; j--)
        if (is_spctk(sc_curchs(sc, j)))
            return sc_readchs(sc, strlen(sc_curchs(sc, j)));
    if (sc_curch(sc) == '.') {
        if (sc_nxtch(sc) >= '0' && sc_nxtch(sc) <= '9')
            return sc_read_num_lit(sc, buff);
        buff_add(buff, sc_readch(sc));
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

/*
 * CAUTION: if you don't read, then rollback,
 *          but shouldn't rollback when read indentations/dedentations!
 */
char *sc_read(scanner *sc) {
    if (sc->skip_newlines) {
        sc->ps = sc->ps2;
        sc->skip[2] = '\n';
    }
    if (sc->in_suite)
        sc->ps = sc->ps2;
    if (sc->rbf) {
        sc->rbf = 0; /* clear rollback flag */
        return sc->lasttk;
    }
    if (sc->eolf)
        sc_getline(sc);

    if (sc->eoff)
        return 0;

    if (sc->indentf || sc->dedentf)
        return 0;
    buffer *buff = buff_init();
    char *retptr = sc_rread(sc, buff);
    if (retptr) {
        if (!strcmp(retptr, "(") || !strcmp(retptr, "[") || !strcmp(retptr, "{")) {
            sc->skip_newlines++;
        }
        else if (!strcmp(retptr, ")") || !strcmp(retptr, "]") || !strcmp(retptr, "}")) {
            sc->skip_newlines--;
        }
    }
    if (!sc->skip_newlines) {
        sc->ps = sc->ps1;
        sc->skip[2] = 0;
    }
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
    if (!strcmp(token, "True"))
        return BOOL_EXPR(1);
    else if (!strcmp(token, "False"))
        return BOOL_EXPR(0);
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
        list *endings = list_node();
        list_append_content(endings, ")");
        list *expr_head = list_node();
        token = sc_read(sc);
        if (!strcmp(token, "yield")) {
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                sc->yield = 1;
                return YIELD_ATOM(0);
            }
            rollback(sc);
            void *expressions = parse_expression_list(sc, endings);
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                sc->yield = 1;
                return YIELD_ATOM(expressions);
            }
        }
        else if (!strcmp(token, ")")) {
            return PARENTH_FORM(expr_head);
        }
        rollback(sc);
        void *expression = parse_expression(sc);
        token = sc_read(sc);
        if (!strcmp(token, ")")) {
            return expression;
        }
        else if(!strcmp(token, ",")) {
            list_append_content(expr_head, expression);
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                return PARENTH_FORM(expr_head);
            }
            rollback(sc);
            list_append_list(expr_head, pa_exprs(sc, endings));
            void *retptr = PARENTH_FORM(expr_head);
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                return retptr;
            }
        }
        else if (!strcmp(token, "for")) {
            rollback(sc);
            void *stmt = YIELD_STMT(expression);
            list *stmts = list_node();
            list_append_content(stmts, parse_comp_for(sc, stmt));
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                return GENERATOR(SUITE(stmts));
            }
        }
    }
    else if (!strcmp(token, "[")) {
        list *endings = list_node();
        list_append_content(endings, "]");
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
            list_append_list(expr_head, pa_exprs(sc, endings));
            token = sc_read(sc);
            if (!strcmp(token, "]"))
                return LIST_EXPR(expr_head);
        }
        else if (!strcmp(token, "for")) {
            rollback(sc);
            void *to_add = LIST_EXPR(expr_head);
            void *right_side = B_EXPR(IDENTIFIER(0), "+", to_add);
            list *tmpl = list_node();
            list_append_content(tmpl, IDENTIFIER(0));
            void *stmt = ASSIGNMENT_STMT(tmpl, right_side);
            list *stmts = list_node();
            list *targets_list = list_node();
            list_append_content(targets_list, IDENTIFIER(0));
            list_append_content(stmts, ASSIGNMENT_STMT(targets_list, LIST_EXPR(list_node())));
            list_append_content(stmts, parse_comp_for(sc, stmt));
            token = sc_read(sc);
            if (!strcmp(token, "]")) {
                return LIST_COMPREHENSION(SUITE(stmts));
            }
        }
    }
    else if (!strcmp(token, "{")) {
        list *endings = list_node();
        list_append_content(endings, "}");
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
            list_append_list(expr_head, pa_exprs(sc, endings));
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
        if (sc->eoff)
            return primary;
        if (!strcmp(token, ".")) {
            token = sc_read(sc);
            primary = ATTRIBUTEREF(primary, IDENTIFIER(token));
        }
        else if (!strcmp(token, "[")) {
            void *val = pa_sll_or_subs(sc);
            token = sc_read(sc);
            if (!strcmp(token, "]")) {
                if (type(val) == slice_expr_t)
                    primary = SLICING(primary, (slice_expr *)val);
                else if (type(val) == subsc_expr_t)
                    primary = SUBSCRIPTION(primary, (subsc_expr *)val);
            }
        }
        else if (!strcmp(token, "(")) {
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                primary = CALL(primary, 0);
            }
            else {
                rollback(sc);
                list *arguments = parse_argument_list(sc, ")");
                token = sc_read(sc);
                if (!strcmp(token, ")")) {
                    primary = CALL(primary, arguments);
                }
            }
        }
        else {
            rollback(sc);
            return primary;
        }
    }
}

list *parse_argument_list(scanner *sc, char *ending) {
    char *token;
    list *retptr = list_node(),
         *target_list = list_node(),
         *expr_list = list_node(),
         *expressions = list_node();
    list_append_content(retptr, target_list);
    list_append_content(retptr, expr_list);
    list_append_content(retptr, expressions);

    while (1) {
        void *expression = parse_expression(sc);
        if (type(expression) == identifier_t) {
            token = sc_read(sc);
            if (!strcmp(token, "=")) {
                list_append_content(target_list, expression);
                list_append_content(expr_list, parse_expression(sc));
                token = sc_read(sc);
                if (!strcmp(token, ",")) {
                    token = sc_read(sc);
                    rollback(sc);
                    if (!strcmp(token, ending))
                        return retptr;
                }
                else if (!strcmp(token, ending)) {
                    rollback(sc);
                    return retptr;
                }
            }
            else if (!strcmp(token, ",")) {
                list_append_content(expressions, expression);
                token = sc_read(sc);
                rollback(sc);
                if (!strcmp(token, ending))
                    return retptr;
            }
            else if (!strcmp(token, ending)) {
                list_append_content(expressions, expression);
                rollback(sc);
                return retptr;
            }
        }
        else {
            token = sc_read(sc);
            if (!strcmp(token, ",")) {
                list_append_content(expressions, expression);
                token = sc_read(sc);
                rollback(sc);
                if (!strcmp(token, ending))
                    return retptr;
            }
            else if (!strcmp(token, ending)) {
                list_append_content(expressions, expression);
                rollback(sc);
                return retptr;
            }
        }
    }
}

void *parse_power(scanner *sc) {
    void *primary = parse_primary(sc);
    char *token = sc_read(sc);
    if (sc->eoff)
        return primary;
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
        if (sc->eoff)
            return expr;
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
        if (sc->eoff)
            return expr;
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
        if (sc->eoff)
            return expr;
        if (strcmp(token, "<<") || !strcmp(token, ">>")) {
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
        if (sc->eoff)
            return expr;
        if (strcmp(token, "&")) {
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
        if (sc->eoff)
            return expr;
        if (strcmp(token, "^")) {
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
        if (sc->eoff)
            return expr;
        if (strcmp(token, "|")) {
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
    if (sc->eoff)
        return or_expr;
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
        if (sc->eoff) {
            comp_expr->comparisons = comparisons;
            return comp_expr;
        }
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
        if (sc->eoff)
            return test;
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
        if (sc->eoff)
            return test;
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
    if (sc->eoff)
        return or_test;
    if (strcmp(token, "if")) {
        rollback(sc);
        return or_test;
    }
    void *or_test2 = parse_or_test(sc);
    token = sc_read(sc);
    if (!strcmp(token, "else"))
        return CONDITIONAL_EXPRESSION(or_test, or_test2, parse_expression(sc));
}

void *parse_lambda_expr(scanner *sc) {
    char *token = sc_read(sc);  // it should be lambda
    list *parameters = list_node();
    list *assign_targets = list_node(), *assign_exprs = list_node();
    expression_list *assign_target_list, *assign_expr_list;
    token = sc_read(sc);
    if (strcmp(token, ":")) {
        rollback(sc);
        while (1) {
            token = sc_read(sc);
            if (is_identifier(token)) {
                identifier *target = IDENTIFIER(token);
                list_append_content(parameters, target);
                token = sc_read(sc);
                if (!strcmp(token, "=")) {
                    list_append_content(assign_targets, target);
                    list_append_content(assign_exprs, parse_expression(sc));
                    token = sc_read(sc);
                }
                if (!strcmp(token, ",")) {
                    token = sc_read(sc);
                    if (!strcmp(token, ":"))
                        break;
                    rollback(sc);
                }
                else if (!strcmp(token, ":"))
                    break;
            }
        }
    }
    if (!list_is_empty(assign_targets)) {
        assign_target_list = EXPRESSION_LIST(assign_targets);
        assign_expr_list = EXPRESSION_LIST(assign_exprs);
    }
    else {
        assign_target_list = 0;
        assign_expr_list = 0;
    }
    return LAMBDA_EXPR(parameters, parse_expression(sc), assign_target_list, assign_expr_list);
}

void *parse_lambda_expr_nocond(scanner *sc) {
    char *token = sc_read(sc);  // it should be lambda
    list *parameters = list_node();
    list *assign_targets = list_node(), *assign_exprs = list_node();
    expression_list *assign_target_list, *assign_expr_list;
    token = sc_read(sc);
    if (strcmp(token, ":")) {
        rollback(sc);
        while (1) {
            token = sc_read(sc);
            if (is_identifier(token)) {
                identifier *target = IDENTIFIER(token);
                list_append_content(parameters, target);
                token = sc_read(sc);
                if (!strcmp(token, "=")) {
                    list_append_content(assign_targets, target);
                    list_append_content(assign_exprs, parse_expression(sc));
                    token = sc_read(sc);
                }
                if (!strcmp(token, ",")) {
                    token = sc_read(sc);
                    if (!strcmp(token, ":"))
                        break;
                    rollback(sc);
                }
                else if (!strcmp(token, ":"))
                    break;
            }
        }
    }
    if (!list_is_empty(assign_targets)) {
        assign_target_list = EXPRESSION_LIST(assign_targets);
        assign_expr_list = EXPRESSION_LIST(assign_exprs);
    }
    else {
        assign_target_list = 0;
        assign_expr_list = 0;
    }
    return LAMBDA_EXPR(parameters, parse_expression_nocond(sc), assign_target_list, assign_expr_list);
}

void *parse_expression(scanner *sc) {
    char *token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "lambda")) {
        return parse_lambda_expr(sc);
    }
    return parse_conditional_expression(sc);
}

void *parse_expression_nocond(scanner *sc) {
    char *token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "lambda")) {
        return parse_lambda_expr_nocond(sc);
    }
    return parse_or_test(sc);
}

list *pa_exprs(scanner *sc, list *endings) {
    char *token;
    list *expr_head = list_node();
    while (1) {
        list_append_content(expr_head, parse_expression(sc));
        token = sc_read(sc);
        if (sc->eoff)
            return expr_head;
        if (!strcmp(token, ",")) {
            token = sc_read(sc);
            rollback(sc);
            list *ptr;
            for (ptr = endings; ptr; ptr = ptr->next)
                if (!strcmp(token, ptr->content))
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
    token = sc_read(sc);
    if (!strcmp(token, ":")) {
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            token = sc_read(sc);
            rollback(sc);
            if (!strcmp(token, "]"))
                return SLICE_EXPR(0, 0, 0);
            stop = parse_expression(sc);
            return SLICE_EXPR(0, 0, stop);
        }
        rollback(sc);
        if (!strcmp(token, "]"))
            return SLICE_EXPR(0, 0, 0);
        stop = parse_expression(sc);
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            token = sc_read(sc);
            rollback(sc);
            if (!strcmp(token, "]"))
                return SLICE_EXPR(0, stop, 0);
            step = parse_expression(sc);
            return SLICE_EXPR(0, stop, step);
        }
        else if (!strcmp(token, "]")) {
            rollback(sc);
            return SLICE_EXPR(0, stop, 0);
        }
    }
    else {
        rollback(sc);
        start = parse_expression(sc);
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            token = sc_read(sc);
            if (!strcmp(token, ":")) {
                token = sc_read(sc);
                rollback(sc);
                if (!strcmp(token, "]"))
                    return SLICE_EXPR(start, 0, 0);
                step = parse_expression(sc);
                return SLICE_EXPR(start, 0, step);
            }
            rollback(sc);
            if (!strcmp(token, "]"))
                return SLICE_EXPR(start, 0, 0);
            stop = parse_expression(sc);
            token = sc_read(sc);
            if (!strcmp(token, "]")) {
                rollback(sc);
                return SLICE_EXPR(start, stop, 0);
            }
            if (!strcmp(token, ":")) {
                token = sc_read(sc);
                rollback(sc);
                if (!strcmp(token, "]"))
                    return SLICE_EXPR(start, stop, 0);
                step = parse_expression(sc);
                return SLICE_EXPR(start, stop, step);
            }
        }
        else {
            rollback(sc);
            return SUBSC_EXPR(start);
        }
    }
}

void *parse_expression_list(scanner *sc, list *endings) {
    void *expression = parse_expression(sc);
    char *token = sc_read(sc);
    if (sc->eoff)
        return expression;
    if (!strcmp(token, ",")) {
        list *expressions = list_node();
        list_append_content(expressions, expression);
        token = sc_read(sc);
        if (sc->eoff)
            return EXPRESSION_LIST(expressions);
        rollback(sc);
        list *ptr;
        for (ptr = endings; ptr; ptr = ptr->next)
            if (!strcmp(token, (char *)ptr->content))
                return EXPRESSION_LIST(expressions);
        list_append_list(expressions, pa_exprs(sc, endings));
        return EXPRESSION_LIST(expressions);
    }
    else {
        rollback(sc);
        return expression;
    }
}

/* actually parse_atom_list */
void *parse_target_list(scanner *sc, list *endings) {
    void *target = parse_primary(sc);
    char *token = sc_read(sc);
    if (!strcmp(token, ",")) {
        token = sc_read(sc);
        rollback(sc);
        list *ptr;
        for (ptr = endings; ptr; ptr = ptr->next)
            if (!strcmp(token, (char *)ptr->content))
                return target;
        list *targets = list_node();
        list_append_content(targets, target);
        while (1) {
            list_append_content(targets, parse_primary(sc));
            token = sc_read(sc);
            if (!strcmp(token, ",")) {
                token = sc_read(sc);
                rollback(sc);
                list *ptr;
                for (ptr = endings; ptr; ptr = ptr->next)
                    if (!strcmp(token, (char *)ptr->content))
                        return EXPRESSION_LIST(targets);
            }
            else {
                rollback(sc);
                return EXPRESSION_LIST(targets);
            }
        }
    }
    else {
        rollback(sc);
        return target;
    }
}

void *parse_comp_for(scanner *sc, void *stmt) {
    char *token = sc_read(sc);  // it should be "for"
    list *suite_list = list_node();
    list *endings = list_node();
    list_append_content(endings, "in");
    void *targets = parse_target_list(sc, endings);
    token = sc_read(sc);
    if (!strcmp(token, "in")) {
        void *or_test_expr = parse_or_test(sc);
        token = sc_read(sc);
        rollback(sc);
        if (!strcmp(token, "for") || !strcmp(token, "if"))
            list_append_content(suite_list, parse_comp_iter(sc, stmt));
        else
            list_append_content(suite_list, stmt);
        return FOR_STMT(targets, or_test_expr, suite_list);
    }
}

void *parse_comp_if(scanner *sc, void *stmt) {
    char *token = sc_read(sc);  // it should be "if"
    list *condition_list = list_node();
    list *suite_list = list_node();
    list_append_content(condition_list, parse_expression_nocond(sc));
    token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "for") || !strcmp(token, "if"))
        list_append_content(suite_list, parse_comp_iter(sc, stmt));
    else
        list_append_content(suite_list, stmt);
    return IF_STMT(condition_list, suite_list);
}

void *parse_comp_iter(scanner *sc, void *stmt) {
    char *token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "for"))
        return parse_comp_for(sc, stmt);
    else if (!strcmp(token, "if"))
        return parse_comp_if(sc, stmt);
}

void *parse_simple_stmt(scanner *sc) {
    char *token = sc_read(sc);
    list *endings = list_node();
    list_append_content(endings, ";");
    list_append_content(endings, "\n");
    if (!strcmp(token, "return")) {
        token = sc_read(sc);
        if (sc->eoff)
            return RETURN_STMT(0);
        rollback(sc);
        if (!strcmp(token, "\n") || !strcmp(token, ";")) {
            return RETURN_STMT(0);
        }
        void *expressions = parse_expression_list(sc, endings);
        return RETURN_STMT(expressions);
    }
    else if (!strcmp(token, "yield")) {
        token = sc_read(sc);
        if (sc->eoff)
            return YIELD_STMT(0);
        rollback(sc);
        if (!strcmp(token, "\n") || !strcmp(token, ";")) {
            sc->yield = 1;
            return YIELD_STMT(0);
        }
        sc->yield = 1;
        return YIELD_STMT(parse_expression_list(sc, endings));
    }
    else if (!strcmp(token, "break")) {
        return BREAK_STMT();
    }
    else if (!strcmp(token, "continue")) {
        return CONTINUE_STMT();
    }
    else if (!strcmp(token, "pass")) {
        return PASS_STMT();
    }
    else if (!strcmp(token, "import")) {
        list *module_name_list = list_node();
        list *as_name_list = list_node();
        while (1) {
            token = sc_read(sc);
            if (is_identifier(token)) {
                list_append_content(module_name_list, token);
                token = sc_read(sc);
                if (!strcmp(token, "as")) {
                    token = sc_read(sc);
                    list_append_content(as_name_list, token);
                    token = sc_read(sc);
                    if (strcmp(token, ",")) {
                        rollback(sc);
                        return IMPORT_STMT(module_name_list, as_name_list);
                    }
                }
                else {
                    list_append_content(as_name_list, strdup(""));
                    if (strcmp(token, ",")) {
                        rollback(sc);
                        return IMPORT_STMT(module_name_list, as_name_list);
                    }
                }
            }
        }
    }
    else if (!strcmp(token, "from")) {
        token = sc_read(sc);
        if (is_identifier(token)) {
            char *module_name = token;
            token = sc_read(sc);
            if (!strcmp(token, "import")) {
                list *var_name_list = list_node();
                list *as_name_list = list_node();
                while (1) {
                    token = sc_read(sc);
                    if (is_identifier(token)) {
                        list_append_content(var_name_list, token);
                        token = sc_read(sc);
                        if (!strcmp(token, "as")) {
                            token = sc_read(sc);
                            list_append_content(as_name_list, token);
                            token = sc_read(sc);
                            if (strcmp(token, ",")) {
                                rollback(sc);
                                return IMPORT_STMT2(module_name, var_name_list, as_name_list);
                            }
                        }
                        else {
                            list_append_content(as_name_list, strdup(""));
                            if (strcmp(token, ",")) {
                                rollback(sc);
                                return IMPORT_STMT2(module_name, var_name_list, as_name_list);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (!strcmp(token, "del")) {
        list *endings = list_node();
        list_append_content(endings, ";");
        list_append_content(endings, "\n");
        return DEL_STMT(parse_target_list(sc, endings));
    }
    else {
        rollback(sc);
        list_append_content(endings, "=");
        void *expression_list1 = parse_expression_list(sc, endings);
        token = sc_read(sc);
        if (sc->eoff)
            return EXPRESSION_STMT(expression_list1);
        if (!strcmp(token, "=")) {
            list *targets_list = list_node();
            list_append_content(targets_list, expression_list1);
            while (1) {
                list *expression_list2 = parse_expression_list(sc, endings);
                token = sc_read(sc);
                if (sc->eoff)
                    return ASSIGNMENT_STMT(targets_list, expression_list2);
                if (!strcmp(token, "="))
                    list_append_content(targets_list, expression_list2);
                else {
                    rollback(sc);
                    return ASSIGNMENT_STMT(targets_list, expression_list2);
                }
            }
        }
        else if (!strcmp(token, "**=")) {
            list *target_list = list_node();
            list_append_content(target_list, expression_list1);
            void *augtarget = expression_list1;
            void *expression_list2 = parse_expression_list(sc, endings);
            void *augvalue = POWER(augtarget, expression_list2);
            return ASSIGNMENT_STMT(target_list, augvalue);
        }
        else if (is_augop(token)) {
            token[strlen(token)-1] = 0;
            list *target_list = list_node();
            list_append_content(target_list, expression_list1);
            void *augtarget = expression_list1;
            void *expression_list2 = parse_expression_list(sc, endings);
            void *augvalue = B_EXPR(augtarget, token, expression_list2);
            return ASSIGNMENT_STMT(target_list, augvalue);
        }
        else {
            rollback(sc);
            return EXPRESSION_STMT(expression_list1);
        }
    }
}

void *parse_stmt_list(scanner *sc) {
    list *stmts = list_node();
    while (1) {
        list_append_content(stmts, parse_simple_stmt(sc));
        char *token = sc_read(sc);
        if (sc->eoff)
            return STMT_LIST(stmts);
        if (!strcmp(token, ";")) {
            token = sc_read(sc);
            if (sc->eoff)
                return STMT_LIST(stmts);
            if (!strcmp(token, "\n"))
                return STMT_LIST(stmts);
            rollback(sc);
        }
        else if (!strcmp(token, "\n"))
            return STMT_LIST(stmts);
    }
}

void *parse_suite(scanner *sc) {
    sc->in_suite++;
    char *token = sc_read(sc);
    if (!strcmp(token, "\n")) {
        if (!sc_read(sc) && sc->indentf--) {
            list *stmts = list_node();
            while (1) {
                list_append_content(stmts, parse_stmt(sc));
                token = sc_read(sc);
                if (sc->eoff) {
                    sc->in_suite--;
                    return SUITE(stmts);
                }
                if (!token && sc->dedentf--) {
                    sc->in_suite--;
                    return SUITE(stmts);
                }
                rollback(sc);
            }
        }
    }
    else {
        rollback(sc);
        void *retptr = parse_stmt_list(sc);
        sc->in_suite--;
        return retptr;
    }
}

void *parse_if_stmt(scanner *sc) {
    list *condition_list = list_node();
    list *suite_list = list_node();
    char *token = sc_read(sc); // it should be "if"
    list_append_content(condition_list, parse_expression(sc));
    token = sc_read(sc);
    if (!strcmp(token, ":")) {
        list_append_content(suite_list, parse_suite(sc));
        while (1) {
            token = sc_read(sc);
            if (!token) {
                return IF_STMT(condition_list, suite_list);
            }
            if (!strcmp(token, "elif")) {
                list_append_content(condition_list, parse_expression(sc));
                token = sc_read(sc);
                if (!strcmp(token, ":")) {
                    list_append_content(suite_list, parse_suite(sc));
                }
            }
            else if (!strcmp(token, "else")) {
                token = sc_read(sc);
                if (!strcmp(token, ":")) {
                    list_append_content(suite_list, parse_suite(sc));
                    return IF_STMT(condition_list, suite_list);
                }
            }
            else {
                rollback(sc);
                return IF_STMT(condition_list, suite_list);
            }
        }
    }
}

void *parse_while_stmt(scanner *sc) {
    list *suite_list = list_node();
    char *token = sc_read(sc);  // it should be "while"
    void *condition = parse_expression(sc);
    token = sc_read(sc);
    if (!strcmp(token, ":")) {
        list_append_content(suite_list, parse_suite(sc));
        token = sc_read(sc);
        if (!token)
            return WHILE_STMT(condition, suite_list);
        else if (!strcmp(token, "else")) {
            token = sc_read(sc);
            if (!strcmp(token, ":")) {
                list_append_content(suite_list, parse_suite(sc));
                return WHILE_STMT(condition, suite_list);
            }
        }
        else {
            rollback(sc);
            return WHILE_STMT(condition, suite_list);
        }
    }
}

void *parse_for_stmt(scanner *sc) {
    char *token = sc_read(sc);  // it should be "for"
    list *suite_list = list_node(), *endings = list_node();
    list_append_content(endings, "in");
    void *targets = parse_target_list(sc, endings);
    token = sc_read(sc);
    if (!strcmp(token, "in")) {
        list_append_content(endings, ":");
        void *expressions = parse_expression_list(sc, endings);
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            list_append_content(suite_list, parse_suite(sc));
            token = sc_read(sc);
            if (!token)
                return FOR_STMT(targets, expressions, suite_list);
            else if (!strcmp(token, "else")) {
                token = sc_read(sc);
                if (!strcmp(token, ":")) {
                    list_append_content(suite_list, parse_suite(sc));
                    return FOR_STMT(targets, expressions, suite_list);
                }
            }
            else {
                rollback(sc);
                return FOR_STMT(targets, expressions, suite_list);
            }
        }
    }
}

void *parse_funcdef(scanner *sc) {
    identifier *id, *target;
    list *parameters;
    list *assign_targets = list_node(), *assign_exprs = list_node();
    char *token = sc_read(sc);  // it should be "def"
    token = sc_read(sc);
    expression_list *assign_target_list, *assign_expr_list;
    if (is_identifier(token)) {
        id = IDENTIFIER(token);
        token = sc_read(sc);
        if (!strcmp(token, "(")) {
            token = sc_read(sc);
            if (!strcmp(token, ")"))
                parameters = 0;
            else {
                rollback(sc);
                parameters = list_node();
                while (1) {
                    token = sc_read(sc);
                    if (is_identifier(token)) {
                        target = IDENTIFIER(token);
                        list_append_content(parameters, target);
                        token = sc_read(sc);
                        if (!strcmp(token, "=")) {
                            list_append_content(assign_targets, target);
                            list_append_content(assign_exprs, parse_expression(sc));
                            token = sc_read(sc);
                        }
                        if (!strcmp(token, ",")) {
                            token = sc_read(sc);
                            if (!strcmp(token, ")"))
                                break;
                            rollback(sc);
                        }
                        else if (!strcmp(token, ")"))
                            break;
                    }
                }
            }
            token = sc_read(sc);
            if (!strcmp(token, ":")) {
                void *_suite = parse_suite(sc);
                int yield = sc->yield;
                sc->yield = 0;
                if (!list_is_empty(assign_targets)) {
                    assign_target_list = EXPRESSION_LIST(assign_targets);
                    assign_expr_list = EXPRESSION_LIST(assign_exprs);
                }
                else {
                    assign_target_list = 0;
                    assign_expr_list = 0;
                }
                return FUNCDEF(id, parameters, _suite, yield, assign_target_list, assign_expr_list);
            }
        }
    }
}

void *parse_classdef(scanner *sc) {
    identifier *id;
    char *token = sc_read(sc);  // it should be "class"
    token = sc_read(sc);
    if (is_identifier(token)) {
        id = IDENTIFIER(token);
        token = sc_read(sc);
        if (!strcmp(token, ":")) {
            return CLASSDEF(id, 0, parse_suite(sc));
        }
        else if (!strcmp(token, "(")) {
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                token = sc_read(sc);
                if (!strcmp(token, ":")) {
                    return CLASSDEF(id, 0, parse_suite(sc));
                }
            }
            rollback(sc);
            list *endings = list_node();
            list_append_content(endings, ")");
            void *expressions = parse_expression_list(sc, endings);
            list *inheritance;
            if (type(expressions) == expression_list_t)
                inheritance = ((expression_list *)expressions)->expr_head;
            else {
                inheritance = list_node();
                list_append_content(inheritance, expressions);
            }
            token = sc_read(sc);
            if (!strcmp(token, ")")) {
                token = sc_read(sc);
                if (!strcmp(token, ":")) {
                    return CLASSDEF(id, inheritance, parse_suite(sc));
                }
            }
        }
    }
}

void *parse_compound_stmt(scanner *sc) {
    char *token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "if"))
        return parse_if_stmt(sc);
    else if (!strcmp(token, "while"))
        return parse_while_stmt(sc);
    else if (!strcmp(token, "for"))
        return parse_for_stmt(sc);
    else if (!strcmp(token, "def"))
        return parse_funcdef(sc);
    else if (!strcmp(token, "class"))
        return parse_classdef(sc);
}

void *parse_stmt(scanner *sc) {
    char *token = sc_read(sc);
    rollback(sc);
    if (!strcmp(token, "if") || !strcmp(token, "while") ||
        !strcmp(token, "for") || !strcmp(token, "def") ||
        !strcmp(token, "class"))
        return parse_compound_stmt(sc);
    return parse_stmt_list(sc);
}

void interpret(FILE *stream, environment *env)
{
    def__builtins__(env);

    def_func(env, _print, "print");
    def_func(env, _next, "next");
    def_func(env, _len, "len");
    def_func(env, _abs, "abs");
    def_int(env);
    def_str(env);
    def_list(env);
    def_range(env);
    def_type(env);

    char *token;

    scanner *sc = sc_init(stream);
    if (sc->eoff)
        return;

    while (1) {
        void *stmt = parse_stmt(sc);
        int pf = 0;
        if (stream == stdin)
            pf = 1;
        execute(stmt, env, pf);
        if (stream == stdin && type(stmt) != stmt_list_t) {
            sc->ps = sc->ps2;
            token = sc_read(sc);  /* token should be "\n" */
            sc->ps = sc->ps1;
        }
        del(stmt);
        token = sc_read(sc);
        while (token && !strcmp(token, "\n"))
            token = sc_read(sc);
        if (!token && sc->eoff) {
            break;
        }
        rollback(sc);
    }
}

int main(int argc, char **argv)
{
    FILE *stream;
    if (argc == 1)
        stream = stdin;
    else if (argc == 2)
        stream = fopen(argv[1], "r");

    environment *global_env = environment_init(0);

    interpret(stream, global_env);

    return 0;
}