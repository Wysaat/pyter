#ifndef EVALUATE_H
#define EVALUATE_H

#include "list.h"
#include "integer.h"
#include "environment.h"

typedef struct suite suite;
typedef struct expression_list expression_list;

typedef struct identifier {
    int type;
    char *value;
} identifier;

typedef struct int_expr {
    int type;
    char *value;
} int_expr;

typedef struct bool_expr {
    int type;
    int value;
} bool_expr;

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
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} set_expr;

typedef struct dict_expr {
    int type;
    list *expr_head;
    list *expr_head2;
    list *ptr;  /* for yield */
    list *ptr2;  /* for yield */
} dict_expr;

typedef struct parenth_form {
    int type;
    list *expr_head;
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} parenth_form;

typedef struct generator {
    int type;
    suite *_suite;
} generator;

typedef struct list_expr {
    int type;
    list *expr_head;
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} list_expr;

typedef struct list_comprehension {
    int type;
    suite *_suite;
} list_comprehension;

typedef struct yield_atom {
    int type;
    void *expressions;
    int yielded;
} yield_atom;

typedef struct attributeref {
    int type;
    void *primary;
    identifier *id;
} attributeref;

typedef struct slice_expr {
    int type;
    void *start;
    void *stop;
    void *step;
    void *start_val; /* for yield */
    void *stop_val;  /* for yield */
    void *step_val;  /* for yield */
} slice_expr;

typedef struct subsc_expr {
    int type;
    void *value;
} subsc_expr;

typedef struct slicing {
    int type;
    void *primary;
    slice_expr *slice;
    void *primary_val;  /* for yield */
} slicing;

typedef struct subscription {
    int type;
    void *primary;
    subsc_expr *subsc;
    void *primary_val;  /* for yield */
} subscription;

typedef struct call {
    int type;
    void *primary;
    list *arguments;  // can be 0
    void *primary_val;  /* for yield */
} call;

typedef struct power {
    int type;
    void *primary;
    void *u_expr;
    void *primary_val;  /* for yield */
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
    void *left_val;  /* for yield */
} b_expr;

typedef struct comparison {
    int type;
    list *comparisons;
    list *list_ptr;  /* for yield */
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
    void *retptr;  /* for yield */
    void *condptr;  /* for yield */
    void *retptr2; /* for yield */
} conditional_expression;

typedef struct lambda_expr {
    int type;
    list *parameters;
    void *expr;
    expression_list *assign_target_list;
    expression_list *assign_expr_list;
} lambda_expr;

struct expression_list {
    int type;
    list *expr_head;
    list *value_list; /* for yield */
    list *expr_ptr;   /* for yield */
};

void *IDENTIFIER(char *);
void *INT_EXPR(char *);
void *BOOL_EXPR(int );
void *FLOAT_EXPR(char *);
void *IMAG_EXPR(char *);
void *STR_EXPR(char *);
void *POWER(void *primary, void *u__expr);
void *PARENTH_FORM(list *);
void *GENERATOR(suite *);
void *LIST_EXPR(list *);
void *SET_EXPR(list *);
void *DICT_EXPR(list *, list *);
void *LIST_COMPREHENSION(suite *);
void *YIELD_ATOM(void *);
void *ATTRIBUTEREF(void *primary, identifier *id);
void *SLICE_EXPR(void *start, void *stop, void *step);
void *SUBSC_EXPR(void *);
void *SLICING(void *primary, slice_expr *);
void *SUBSCRIPTION(void *primary, subsc_expr *);
void *CALL(void *primary, list *arguments);
void *U_EXPR(char *, void *);
void *B_EXPR(void *, char *, void *);
void *NOT_TEST(void *);
void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr);
void *LAMBDA_EXPR(list *parameters, void *expr, expression_list *assign_target_list, expression_list *assign_expr_list);
void *EXPRESSION_LIST(list *);
void *PYINT(integer *);
void *PYSTR(char *);

void *identifierEvaluate(identifier *, environment *);
void *int_exprEvaluate(int_expr *);
void *bool_exprEvalaute(bool_expr *);
void *float_exprEvaluate(float_expr *);
void *imag_exprEvaluate(imag_expr *);
void *str_exprEvaluate(str_expr *);
void *parenth_formEvaluate(parenth_form *, environment *);
void *generatorEvaluate(generator *, environment *);
void *list_exprEvaluate(list_expr *, environment *);
void *set_exprEvaluate(set_expr *, environment *);
void *dict_exprEvaluate(dict_expr *, environment *);
void *list_comprehensionEvaluate(list_comprehension *, environment *);
void *yield_atomEvaluate(yield_atom *, environment *);
void *attributerefEvaluate(attributeref *, environment *);
void *slice_exprEvaluate(slice_expr *, environment *);
void *slicingEvaluate(slicing *, environment *);
void *subscriptionEvaluate(subscription *, environment *);
void *callEvaluate(call *, environment *);
void *powerEvaluate(power *, environment *);
void *u_exprEvaluate(u_expr *, environment *);
void *b_exprEvaluate(b_expr *, environment *);
void *not_testEvaluate(not_test *, environment *);
void *comparisonEvaluate(comparison *, environment *);
void *conditional_expressionEvaluate(conditional_expression *, environment *);
void *lambda_exprEvaluate(lambda_expr *, environment *);
void *expression_listEvaluate(expression_list *, environment *);
void *evaluate(void *, environment *);
void print_nnl(void *);
void print(void *);

void idenfitier_del(void *);
void int_expr_del(void *);
void bool_expr_del(void *);
void float_expr_del(void *);
void imag_expr_del(void *);
void str_expr_del(void *);
void parenth_form_del(void *);
void generator_del(void *);
void list_expr_del(void *);
void set_expr_del(void *);
void dict_expr_del(void *);
void list_comprehension_del(void *);
void yield_atom_del(void *);
void attributeref_del(void *);
void slice_expr_del(void *);
void slicing_del(void *);
void subsc_expr_del(void *vptr);
void subscription_del(void *);
void call_del(void *);
void power_del(void *);
void u_expr_del(void *);
void b_expr_del(void *);
void not_test_del(void *);
void comparison_del(void *);
void conditional_expression_del(void *);
void lambda_expr_del(void *);
void expression_list_del(void *);

#endif /* EVALUATE_H */