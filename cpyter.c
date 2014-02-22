#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "cpyter.h"

/* "<>" is deprecated in Python 3 */
char tokens[100][4] = { "+", "-", "*", "**", "/", "//", "%",
                        "<<", ">>", "&", "|", "^", "~",
                        "<", ">", "<=", ">=", "==", "!=", "<>",
                        "(", ")", "[", "]", "{", "}", "@",
                        ",", ":", "`", "=", ";",
                        "+=", "-=", "*=", "/=", "//=", "%=",
                        "&=", "|=", "^=", ">>=", "<<=", "**=",

                        "!", "$", "?", "#", "\\",

                        "+=", "-=", "*=", "/=", "//=", "%=", "**=",
                        ">>=", "<<=", "&=", "^=", "|=", " " };

char stringprefixes[10][2] = { "r", "u", "R", "U", " " };

char comp_operators[10][3] = { "<", ">", "==", ">=", "<=", "<>", "!=", " " };

struct item {
    char content[ITEMSIZE];
    struct item *next;
};

struct {
    char string[STRINGSIZE];
    int index;
    int line_number;
    struct item items_head;
    int multi_lines;
    int rewind;
    char last_item[ITEMSIZE];
} global = {
    .string = "",
    .index = 0,
    .line_number = 0,
    .items_head = { " ", 0 },
    .multi_lines = 0,
    .rewind = 0,
    .last_item = "",
};

void remonter() {
    global.rewind = 1;
    char *item = pop_item();
    strcpy(global.last_item, item);
}

void add_item(char *word) {
    struct item *ptr = &global.items_head;
    struct item *new_item = (struct item *)malloc(sizeof(struct item));
    strcpy(new_item->content, word);
    new_item->next = 0;
    while (ptr->next != 0)
        ptr = ptr->next;
    ptr->next = new_item;
}

char *pop_item() {
    struct item *ptr = &global.items_head;
    struct item *last;
    while (ptr->next != 0) {
        last = ptr;
        ptr = ptr->next;
    }
    last->next = 0;
    return ptr->content;
}

void show_items() {
    struct item *ptr = &global.items_head;
    printf("[");
    while (ptr->next != 0) {
        printf("'%s', ", ptr->next->content);
        ptr = ptr->next;
    }
    printf("]\n");
}

char *last_item() {
    struct item *ptr = &global.items_head;
    while (ptr->next != 0)
        ptr = ptr->next;
    return ptr->content;
}

void interactive_get_line() {
    printf(">>> ");
    gets(global.string);
    global.index = 0;
}

void read_numeric_literal(char *item) {
    int i = 0, dot_count = 0;
    while ((global.string[global.index] >= '0' && global.string[global.index] <= '9') ||
                (global.string[global.index] == '.' && !dot_count)) {
        if (global.string[global.index] == '.')
            dot_count++;
        item[i++] = global.string[global.index++];
    }
    item[i] = 0;
    add_item(item);
    return;
}

void read_string_literal(char *item) {
    int i = 1;
    char op = global.string[global.index++];
    item[0] = op;
    while (global.string[global.index] != op)
        item[i++] = global.string[global.index++];
    item[i++] = global.string[global.index++];
    item[i] = 0;
    add_item(item);
    return;
}

void raw_read(char *item) {
    int i, j, k = 0;

    if (global.index < strlen(global.string)) {
        while (global.string[global.index] == ' ' || global.string[global.index] == '\t') {
            global.index++;
        }
        for (j = 3; j > 0; j--) {
            for (i = 0; strcmp(tokens[i], " ") != 0; i++) {
                if (strncmp(global.string+global.index, tokens[i], j) == 0) {
                    strncpy(item, global.string+global.index, j);
                    item[j] = 0;
                    add_item(item);
                    global.index += j;
                    return;
                }
            }
        }
        if (global.string[global.index] == '.') {
            if (global.string[global.index+1] >= '0' && global.string[global.index+1] <= '9') {
                read_numeric_literal(item);
                return;
            }
            else {
                strncpy(item, global.string+global.index, 1);
                item[1] = 0;
                add_item(item);
                global.index++;
                return;
            }
        }
        if (global.string[global.index] >= '0' && global.string[global.index] <= '9') {
            read_numeric_literal(item);
            return;
        }
        if (global.string[global.index] == '"' || global.string[global.index] == '\'') {
            read_string_literal(item);
            return;
        }
        while ((global.string[global.index] >= 'a' && global.string[global.index] <= 'z') ||
               (global.string[global.index] >= 'A' && global.string[global.index] <= 'Z') ||
               (global.string[global.index] >= '0' && global.string[global.index] <= '9') ||
               (global.string[global.index] == '_')) {
            item[k++] = global.string[global.index++];
        }
        for (i = 0; strcmp(stringprefixes[i], " ") != 0; i++) {
            if ((strcmp(stringprefixes[i], item) == 0) && 
                     (global.string[global.index == '"' || global.string[global.index == '\'']])) {
                read_string_literal(item+strlen(item));
                add_item(item);
                return;
            }
            else {
                item[k] = 0;
                add_item(item);
                return;
            }
        }
    }
    else {
        item[0] = 0;
        add_item(item);
        return;
    }
}

void read(char *item) {
    if (global.rewind) {
        global.rewind = 0;
        strcpy(item, global.last_item);
        add_item(global.last_item);
        return;
    }
    raw_read(item);
}

int is_digit(char ch) {
    if (ch >= '0' && ch <= '9')
        return 1;
    return 0;
}

int is_alpha(char ch) {
    if ((ch >= 'a' && ch <= 'z') || ch >= 'A' && ch <= 'Z')
        return 1;
    return 0;
}

int is_id(char *item) {
    int i = 0;
    if (!(is_alpha(item[0]) || item[0] == '_'))
        return 0;
    while (item[++i] != 0) {
        if (!(is_alpha(item[i]) || item[i] == '_' || is_digit(item[i])))
            return 0;
    }
    return 1;
}

int is_int(char *item) {
    int i;
    for (i = 0; item[i]; i++) {
        if (!is_digit(item[i]))
            return 0;
    }
    if (i == 0)
        return 0;
    return 1;
}

int is_str(char *item) {
    int i;
    if (item[0] == '\'' || item[0] == '"')
        return 1;
    return 0;
}

void *parse_atom() {
    char item[ITEMSIZE];
    read(item);
    if (is_int(item)) {
        int_expr *retval = (int_expr *)malloc(sizeof(int_expr));
        retval->type = int_expr_t;
        strcpy(retval->value, item);
        return retval;
    }
    else if (is_str(item)) {
        str_expr *retval = (str_expr *)malloc(sizeof(str_expr));
        retval->type = str_expr_t;
        strcpy(retval->value, item);
        return retval;
    }
}

void *parse_primary() {
    void *primary = parse_atom();
    return primary;
}

void *parse_power() {
    char item[ITEMSIZE];
    void *primary = parse_primary();
    read(item);
    if (!match(item, "**")) {
        remonter();
        return primary;
    }
    void *u_expr = parse_u_expr();
    power *retptr = (power *)malloc(sizeof(power));
    retptr->type = power_t;
    retptr->primary = primary;
    retptr->u_expr = u_expr;
    return retptr;
}

void *parse_u_expr() {
    char item[ITEMSIZE];
    read(item);
    if (match(item, "+") || match(item, "-") || match(item, "~")) {
        // void *expr = parse_u_expr();
        u_expr *retptr = (u_expr *)malloc(sizeof(u_expr));
        retptr->type = u_expr_t;
        retptr->u_op = item[0];
        retptr->expr = parse_u_expr();
        return retptr;
    }
    remonter();
    return parse_power();
}

void *parse_m_expr() {
    char item[ITEMSIZE];
    void *expr = parse_u_expr();
    while (1) {
        read(item);
        if (!(match(item, "*") || match(item, "//") ||
              match(item, "/") || match(item, "%"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_u_expr());
    }
}

void *parse_a_expr() {
    char item[ITEMSIZE];
    void *expr = parse_m_expr();
    int *val = evaluate(expr);
    while (1) {
        read(item);
        if (!(match(item, "+") || match(item, "-"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_m_expr());
    }
}

void *parse_shift_expr() {
    char item[ITEMSIZE];
    void *expr = parse_a_expr();
    while (1) {
        read(item);
        if (!(match(item, "<<") || match(item, ">>"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_a_expr());
    }
}

void *parse_and_expr() {
    char item[ITEMSIZE];
    void *expr = parse_shift_expr();
    while (1) {
        read(item);
        if (!(match(item, "&"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_shift_expr());
    }
}

void *parse_xor_expr() {
    char item[ITEMSIZE];
    void *expr = parse_and_expr();
    while (1) {
        read(item);
        if (!(match(item, "^"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_and_expr());
    }
}

void *parse_or_expr() {
    char item[ITEMSIZE];
    void *expr = parse_xor_expr();
    while (1) {
        read(item);
        if (!(match(item, "|"))) {
            remonter();
            return expr;
        }
        expr = B_EXPR(expr, item, parse_xor_expr());
    }
}

int in_comp_operator(char *item) {
    if (match(item, "is") || match(item, "not") || match(item, "in"))
        return 1;
    int i = 0;
    for (i = 0; !match(comp_operators[i], " "); i++) {
        if (match(item, comp_operators[i]))
            return 1;
    }
    return 0;
}

void *parse_comparison() {
    char item[ITEMSIZE];
    char *op;
    void *or_expr = parse_or_expr();
    read(item);
    remonter();
    if (!in_comp_operator(item))
        return or_expr;
    void *left = or_expr;
    void *right;
    list *comparisons = (list *)malloc(sizeof(list));
    bzero(comparisons, sizeof(list));
    comparison *comp_expr = (comparison *)malloc(sizeof(comparison));
    comp_expr->type = comparison_t;
    while (1) {
        read(item);
        if (match(item, "is")) {
            read(item);
            if (match(item, "not"))
                op = "is not";
            else {
                remonter();
                op = "is";
            }
        }
        else if (match(item, "not")) {
            read(item);
            if (match(item, "in"))
                op = "not in";
        }
        else if (match(item, "in"))
            op = "in";
        else if (in_comp_operator(item))
            op = item;
        else {
            remonter();
            comp_expr->comparisons = comparisons;
            return comp_expr;
        }
        right = parse_or_expr();
        list_append(comparisons, B_EXPR(left, op, right));
        left = right;
    }
}

void *parse_not_test() {
    char item[ITEMSIZE];
    read(item);
    if (match(item, "not")) {
        not_test *test = (not_test *)malloc(sizeof(not_test));
        test->type = not_test_t;
        test->expr = parse_not_test();
        return test;
    }
    remonter();
    return parse_comparison();
}

void *parse_and_test() {
    char item[ITEMSIZE];
    void *test = parse_not_test();
    while (1) {
        read(item);
        if (!match(item, "and")) {
            remonter();
            return test;
        }
        test = B_EXPR(test, item, parse_not_test());
    }
}

void *parse_or_test() {
    char item[ITEMSIZE];
    void *test = parse_and_test();
    while (1) {
        read(item);
        if (!match(item, "or")) {
            remonter();
            return test;
        }
        test = B_EXPR(test, item, parse_and_test());
    }
}

void *parse_conditional_expression() {
    char item[ITEMSIZE];
    void *or_test = parse_or_test();
    read(item);
    if (!match(item, "if")) {
        remonter();
        return or_test;
    }
    void *or_test2 = parse_or_test();
    read(item);
    if (match(item, "else")) {
        void *expr = parse_expression();
        conditional_expression *expression = (conditional_expression *)malloc(sizeof(conditional_expression));
        expression->type = conditional_expression_t;
        expression->or_test = or_test;
        expression->or_test2 = or_test2;
        expression->expr = expr;
        return expression;
    }
}

/* no lambda expression support */
void *parse_expression() {
    return parse_conditional_expression();
}

/* no lambda expression support */
void *parse_expression_nocond() {
    return parse_or_test();
}

void *parse_expression_list(char *ending) {
    char item[ITEMSIZE];
    list *expr_head = (list *)malloc(sizeof(list));
    expression_list *retptr = (expression_list *)malloc(sizeof(expression_list));
    retptr->type = expression_list_t;
    while (1) {
        list_append(expr_head, parse_expression());
        read(item);
        if (match(item, ",")) {
            read(item);
            remonter();
            if (match(item, ending)) {
                retptr->expr_head = expr_head;
                return retptr;
            }
        }
        else {
            remonter();
            retptr->expr_head = expr_head;
            return retptr;
        }
    }
}

int test1()
{
    char item[ITEMSIZE];
    interactive_get_line();
    void *expr = parse_expression();
    void *retval = evaluate(expr);
    print(retval);
    return 0;
}

int test()
{
    char item[ITEMSIZE];
    do {
        interactive_get_line();
        puts(global.string);
        printf("[");
        while (global.index < strlen(global.string)) {
            read(item);
            printf("'%s', ", item);
        }
        printf("]\n");
    } while (strcmp(global.string, "exit") != 0);

    return 0;
}

int main()
{
    test1();
    return 0;
}