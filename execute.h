#ifndef EXECUTE_H
#define EXECUTE_H

#include "list.h"
#include "integer.h"

typedef struct list list;

typedef struct identifier {
    int type;
    char *value;
} identifier;

typedef struct int_expr {
    int type;
    char *value;
} int_expr;

typedef struct float_expr {
    int type;
    char *value;
} float_expr;

typedef struct imag_expr {
    int type;
    char *value;
} imag_expr;

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

typedef struct slice_expr {
    int type;
    void *start;
    void *stop;
    void *step;
} slice_expr;

typedef struct subsc_expr {
    int type;
    void *value;
} subsc_expr;

typedef struct slicing {
    int type;
    void *primary;
    slice_expr *slice;
} slicing;

typedef struct subscription {
    int type;
    void *primary;
    subsc_expr *subsc;
} subscription;

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

// typedef struct assignment_stmt {
//     int type;
//     expression_list *targets;
//     expression_list *expressions;
// } assignment_stmt;

// typedef struct environment {
//     list *val_dict;
// } environment;

// typedef struct val_dict_entry {
//     char *id;
//     void *value;
// } val_dict;

int type(void *);

void *IDENTIFIER(char *);
void *INT_EXPR(char *);
void *FLOAT_EXPR(char *);
void *IMAG_EXPR(char *);
void *STR_EXPR(char *);
void *POWER(void *primary, void *u__expr);
void *PARENTH_FORM(list *);
void *LIST_EXPR(list *);
void *SET_EXPR(list *);
void *DICT_EXPR(list *, list *);
void *SLICE_EXPR(void *start, void *stop, void *step);
void *SUBSC_EXPR(void *);
void *SLICING(void *primary, slice_expr *);
void *SUBSCRIPTION(void *primary, subsc_expr *);
void *U_EXPR(char *, void *);
void *B_EXPR(void *, char *, void *);
void *NOT_TEST(void *);
void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr);
void *EXPRESSION_LIST(list *);
// void *ASSIGNMENT_STMT(expression_list *, expression_list *);
void *PYINT(integer *);
void *PYSTR(char *);

// environment *environment_init();

#endif /* EXECUTE_H */