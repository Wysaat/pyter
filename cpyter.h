#ifndef CPYTER_H
#define CPYTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "string.h"
#include "integer.h"
#include "list.h"

#include "scan/buffer.h"
#include "scan/scanner.h"
#include "scan/token.h"

#include "pytype/pyint.h"
#include "pytype/pystr.h"
#include "pytype/pybool.h"
#include "pytype/pytuple.h"
#include "pytype/pyset.h"
#include "pytype/pydict.h"

typedef struct list list;

typedef struct int_expr {
    int type;
    char *value;
} int_expr;

typedef struct str_expr {
    int type;
    char *value;
} str_expr;

typedef struct set_expr {
    int type;
    list *expr_head;
} set_expr;

typedef struct dict_expr {
    int type;
    list *expr_head;
    list *expr_head2;
} dict_expr;

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
    char *op;
    void *expr;
} u_expr;

typedef struct b_expr {
    int type;
    char *op;
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

typedef struct pylist {
    int type;
    list *values;
} pylist;

int is_alph(char );
int is_num(char );
int is_alphnum(char );

void *INT_EXPR(char *);
void *STR_EXPR(char *);
void *POWER(void *primary, void *u__expr);
void *SET_EXPR(list *);
void *DICT_EXPR(list *, list *);
void *U_EXPR(char *, void *);
void *B_EXPR(void *, char *, void *);
void *NOT_TEST(void *);
void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr);
void *EXPRESSION_LIST(list *);
void *PYINT(integer *);
void *PYSTR(char *);

void *PARENTH_FORM(list *);
void *LIST_EXPR(list *);

void *int_exprEvaluate(int_expr *);
void *str_exprEvaluate(str_expr *);
void *parenth_formEvaluate(parenth_form *);
void *list_exprEvaluate(list_expr *);
void *set_exprEvaluate(set_expr *);
void *dict_exprEvaluate(dict_expr *);
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
list **pa_dict_items(scanner *);

char *itoa(int );
char *lltoa(long long );

#include "pytype/pylist.h"

#endif /* CPYTER_H */