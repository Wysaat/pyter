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
    mem_block *content;
    struct item *next;
};

struct {
    mem_block *string;
    int string_sz;
    int index;
    int line_number;
    struct item items_head;
    int multi_lines;
    int rewind;
    mem_block *last_item;
} global = {
    .string = 0,
    .string_sz = 0,
    .index = 0,
    .line_number = 0,
    .items_head = { " ", 0 },
    .multi_lines = 0,
    .rewind = 0,
    .last_item = 0,
};

void remonter() {
    global.rewind = 1;
    global.last_item = pop_item();
}

void add_item(mem_block *content) {
    struct item *ptr = &global.items_head;
    struct item *new_item = (struct item *)malloc(sizeof(struct item));
    new_item->content = content;
    new_item->next = 0;
    while (ptr->next != 0)
        ptr = ptr->next;
    ptr->next = new_item;
}

mem_block *pop_item {
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
        printf("'");
        mem_print(ptr->next->content);
        printf("', ");
        ptr = ptr->next;
    }
    printf("]\n");
}

void interactive_get_line() {
    printf(">>> ");
    global.string = mem_head();
    char *dest = (global.string)->mem;
    mem_block *last_block = global.string;
    while (fgets(dest, MEM_BLOCK_SZ, stdin)) {
        int i;
        for (i = 0; i < MEM_BLOCK_SZ; i++) {
            if (dest[i] == '\n') {
                global.string_sz += strlen(dest);
                global.index = 0;
                return;
            }
        }
        global.string_sz += strlen(dest);
        mem_block *new_block = (mem_block *)malloc(sizeof(mem_block));
        new_block->prev = last_block;
        last_block->next = new_block;
        new_block->next = 0;
        dest = new_block->mem;
        last_block = new_block;
    }
    global.index = 0;
}

void read_numeric_literal(mem_block *item) {
    int i = 0, dot_count = 0;
    while ((mem_subscription(global.string, global.index) >= '0' && mem_subscription(global.string, global.index) <= '9') ||
                (mem_subscription(global.string, global.index) == '.' && !dot_count)) {
        if (mem_subscription(global.string, global.index) == '.')
            dot_count++;
        item[i++] = mem_subscription(global.string, global.index++);
    }
    item[i] = 0;
    add_item(item);
    return;
}

void read_string_literal(mem_block *item) {
    int i = 1;
    char op = mem_subscription(global.string, global.index++);
    item[0] = op;
    while (mem_subscription(global.string, global.index) != op)
        item[i++] = mem_subscription(global.string, global.index++);
    item[i++] = mem_subscription(global.string, global.index++);
    item[i] = 0;
    add_item(item);
    return;
}

void raw_read(mem_block *item) {
    int i, j, k = 0;

    if (global.index < global.string_sz-1) {
        while (mem_subscription(global.string, global.index) == ' ' || mem_subscription(global.string, global.index) == '\t') {
            global.index++;
        }
        for (j = 3; j > 0; j--) {
            for (i = 0; strcmp(tokens[i], " ") != 0; i++) {
                if (mem_ncmp(tokens[i], global.string, global.index, j) == 0) {
                    mem_ncpy_out(item, global.string, global.index, j);
                    item[j] = 0;
                    add_item(item);
                    global.index += j;
                    return;
                }
            }
        }
        if (mem_subscription(global.string, global.index) == '.') {
            if (mem_subscription(global.string, global.index+1) >= '0' && mem_subscription(global.string, global.index+1) <= '9') {
                read_numeric_literal(item);
                return;
            }
            else {
                mem_ncpy_out(item, global.string, global.index, 1);
                item[1] = 0;
                add_item(item);
                global.index++;
                return;
            }
        }
        if (mem_subscription(global.string, global.index) >= '0' && mem_subscription(global.string, global.index) <= '9') {
            read_numeric_literal(item);
            return;
        }
        if (mem_subscription(global.string, global.index) == '"' || mem_subscription(global.string, global.index) == '\'') {
            read_string_literal(item);
            return;
        }
        while ((mem_subscription(global.string, global.index) >= 'a' && mem_subscription(global.string, global.index) <= 'z') ||
               (mem_subscription(global.string, global.index) >= 'A' && mem_subscription(global.string, global.index) <= 'Z') ||
               (mem_subscription(global.string, global.index) >= '0' && mem_subscription(global.string, global.index) <= '9') ||
               (mem_subscription(global.string, global.index) == '_')) {
            item[k++] = mem_subscription(global.string, global.index++);
        }
        for (i = 0; !match(stringprefixes[i], " "); i++) {
            if ((match(stringprefixes[i], item)) &&
                     (mem_subscription(global.string, global.index) == '"' || mem_subscription(global.string, global.index) == '\'')) {
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

void read(mem_block *item) {
    if (global.rewind) {
        global.rewind = 0;
        item = global.last_item;
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

int is_id(mem_block *item){
    int i = 0;
    if (!(is_alpha(mem_subscription(item, 0)) || 
          mem_subscription(item, 0) == '_'))
        return 0;
    while (mem_subscription(item, ++i) != 0) {
        if (!(is_alpha(mem_subscription(item, 0)) ||
            is_digit(mem_subscription(item, i))))
            return 0;
    }
    return 1;
}

int is_int(mem_block *item) {
    int i;
    for (i = 0; mem_subscription(item, i); i++) {
        if (!is_digit(mem_subscription(item, i)))
            return 0;
    }
    if (i == 0)
        return 0;
    return 1;
}

int is_str(mem_block *item) {
    int i;
    if (mem_subscription(item, 0) == '\'' ||
        mem_subscription(item, 0) == '"')
        return 1;
    return 0;
}

void *parse_atom() {
    mem_block *item = mem_head();
    read(item);
    if (is_int(item)) {
        int_expr *retptr = (int_expr *)malloc(sizeof(int_expr));
        retptr->type = int_expr_t;
        strcpy(retptr->value, item);
        return retptr;
    }
    else if (is_str(item)) {
        str_expr *retptr = (str_expr *)malloc(sizeof(str_expr));
        retptr->type = str_expr_t;
        strcpy(retptr->value, item);
        return retptr;
    }
    else if (match(item, "(")) {
        list *expr_head = (list *)malloc(sizeof(list));
        read(item);
        if (match(item, ")"))
            return PARENTH_FORM(expr_head);
        remonter();
        void *expression = parse_expression();
        read(item);
        if (match(item, ")"))
            return expression;
        else if(match(item, ",")) {
            list_append(expr_head, expression);
            read(item);
            if (match(item, ")"))
                return PARENTH_FORM(expr_head);
            remonter();
            list_add(expr_head, pa_exprs(")"));
            void *retptr = PARENTH_FORM(expr_head);
            read(item);
            if (match(item, ")"))
                return retptr;
        }
    }
}

void *parse_primary() {
    void *primary = parse_atom();
    return primary;
}

void *parse_power() {
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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
    mem_block *item = mem_head();
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

list *pa_exprs(char *ending) {
    mem_block *item = mem_head();
    list *expr_head = (list *)malloc(sizeof(list));
    while (1) {
        list_append(expr_head, parse_expression());
        read(item);
        if (match(item, ",")) {
            read(item);
            remonter();
            if (match(item, ending))
                return expr_head;
        }
        else {
            remonter();
            return expr_head;
        }
    }
}

void *parse_expression_list(char *ending) {
    mem_block *item = mem_head();
    expression_list *retptr = (expression_list *)malloc(sizeof(expression_list));
    retptr->type = expression_list_t;
    retptr->expr_head = pa_exprs(ending);
    return retptr;
}

int test2()
{
    interactive_get_line();
    mem_print(global.string);
    return 0;
}

int test1()
{
    mem_block *item = mem_head();
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
        mem_print(global.string);
        printf("[");
        while (global.index < global.string_sz - 1) {
            read(item);
            printf("'%s', ", item);
        }
        printf("]\n");
    } while (mem_ncmp("exit", global.string, 0, 4) != 0);

    return 0;
}

int main()
{
    test1();
    return 0;
}