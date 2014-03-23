#ifndef CPYTER_H
#define CPYTER_H

#define ITEMSIZE 10
#define INTEGER_SZ 5

#define match(x, y) !strcmp(x, y)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "string.h"
#include "scan/buffer.h"
#include "scan/scanner.h"
#include "scan/token.h"

enum types { int_expr_t, str_expr_t,
             parenth_form_t, list_expr_t,
             power_t, u_expr_t, b_expr_t, comparison_t, 
             not_test_t, conditional_expression_t,
             expression_list_t,
             pyint_t, pystr_t, pybool_t,
             pytuple_t, pylist_t, };

typedef struct list list;
typedef struct integer integer;

typedef struct int_expr {
    int type;
    string *value;
} int_expr;

typedef struct str_expr {
    int type;
    string *value;
} str_expr;

typedef struct parenth_form {
    int type;
    list *expr_head;
} parenth_form;

typedef struct list_expr {
    int type;
    list *expr_head;
} list_expr;

typedef struct power {
    int type;
    void *primary;
    void *u_expr;
} power;

typedef struct u_expr {
    int type;
    string *op;
    void *expr;
} u_expr;

typedef struct b_expr {
    int type;
    string *op;
    void *left;
    void *right;
} b_expr;

typedef struct comparison {
    int type;
    list *comparisons;
} comparison;

typedef struct not_test {
    int type;
    void *expr;
} not_test;

typedef struct conditional_expression {
    int type;
    void *or_test;
    void *or_test2;
    void *expr;
} conditional_expression;

typedef struct expression_list {
    int type;
    list *expr_head;
} expression_list;

struct list {
    void *content;
    struct list *prev;
    struct list *next;
};

typedef struct pyint pyint;

typedef struct pystr {
    int type;
    string *value;
} pystr;

typedef struct pybool {
    int type;
    int value;
} pybool;

typedef struct pytuple {
    int type;
    list *values;
} pytuple;

typedef struct pylist {
    int type;
    list *values;
} pylist;

struct integer {
    int value;
    struct integer *higher;
    struct integer *lower;
    char sign;
    int index;
};

int is_alph(char );
int is_num(char );
int is_alphnum(char );

list *list_node();
int list_is_empty(list *);
void list_append_content(list *, void *);
void list_append_list(list *, list *);
list *list_add(list *, list *);
void list_sort(list *, int (*func)());
void list_sort0(list *, int (*func)(), int size);

void *INT_EXPR(string *);
void *STR_EXPR(string *);
void *POWER(void *primary, void *u__expr);
void *U_EXPR(string *, void *);
void *B_EXPR(void *, string *, void *);
void *NOT_TEST(void *);
void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr);
void *EXPRESSION_LIST(list *);
void *PYINT(integer *);
void *PYSTR(string *);
void *PYBOOL(int);
void *PARENTH_FORM(list *);
void *LIST_EXPR(list *);

void *int_exprEvaluate(int_expr *);
void *str_exprEvaluate(str_expr *);
void *powerEvaluate(power *);
void *u_exprEvaluate(u_expr *);
void *b_exprEvaluate(b_expr *);
void *not_testEvaluate(not_test *);
void *comparisonEvaluate(comparison *);
void *conditional_expressionEvaluate(conditional_expression *);
void *expression_listEvaluate(expression_list *);
void *evaluate(void *);
void print_nnl(void *);
void print(void *);

void *parse_u_expr(scanner *sc);
void *parse_expression(scanner *sc);
list *pa_exprs(scanner *, char *);

pystr *pystr__mul__(pystr *, pyint *);
pystr *pystr__add__(pystr *, pystr *);

char *itoa(int );
char *lltoa(long long );

pytuple *pytuple__init__();
pytuple *pytuple__add__(pytuple *, pytuple *);

#include "integer.h"

#include "pytype/pyint.h"
#include "pytype/pylist.h"

#endif /* CPYTER_H */