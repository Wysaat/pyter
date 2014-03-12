#ifndef CPYTER_H
#define CPYTER_H

#define ITEMSIZE 10
#define MEM_BLOCK_SZ 10
#define INTEGER_SZ 5

#define match(x, y) !strcmp(x, y)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum types { int_expr_t, str_expr_t,
             parenth_form_t,
             power_t, u_expr_t, b_expr_t, comparison_t, 
             not_test_t, conditional_expression_t,
             expression_list_t, 
             pyint_t, pystr_t, pybool_t,
             pytuple_t, };

typedef struct list list;
typedef struct mem_block mem_block;
typedef struct integer integer;

typedef struct int_expr {
    int type;
    mem_block *value;
} int_expr;

typedef struct str_expr {
    int type;
    mem_block *value;
} str_expr;

typedef struct parenth_form {
    int type;
    list *expr_head;
} parenth_form;

typedef struct power {
    int type;
    void *primary;
    void *u_expr;
} power;

typedef struct u_expr {
    int type;
    char u_op;
    void *expr;
} u_expr;

typedef struct b_expr {
    int type;
    mem_block *op;
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

typedef struct pyint {
    int type;
    integer *value;
} pyint;

typedef struct pystr {
    int type;
    mem_block *value;
} pystr;

typedef struct pybool {
    int type;
    int value;
} pybool;

typedef struct pytuple {
    int type;
    list *values;
} pytuple;

struct mem_block {
    char mem[MEM_BLOCK_SZ];
    struct mem_block *prev;
    struct mem_block *next;
};

struct integer {
    int value;
    struct integer *higher;
    struct integer *lower;
    char sign;
    int index;
};

list *list_node();
void list_append_content(list *, void *);
void list_append_list(list *, list *);

void *B_EXPR(void *, mem_block *, void *);
void *PYINT(integer *);
void *PYSTR(mem_block *);
void *PYBOOL(int);
void *PARENTH_FORM(list *);

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
void print(void *);

mem_block *pop_item();
void *parse_u_expr();
void *parse_expression();
list *pa_exprs(char *);

pystr *str__mul__(pystr *, pyint *);
pystr *str__add__(pystr *, pystr *);

void mem_print(mem_block *block);
mem_block *mem_head();
char mem_subscription(mem_block *, int );
char *mem_get(mem_block *, int );
mem_block *mem_slice(mem_block *block, int start, int stop);
void mem_alloc(mem_block *);
void mem_set(mem_block *, int ,char);
void mem_del(mem_block *, int index);
void mem_delete(mem_block *, int start, int stop);
int mem_size(mem_block *);
void mem_ncpy_out(char *, mem_block *, int, int);
int mem_ncmp(char *, mem_block *, int, int);
void mem_ncpy(mem_block *, mem_block *, int, int, int);
void mem_insert(mem_block *, mem_block *, int, int, int);
void mem_cpy(mem_block *, mem_block *);
int mem_match_str(mem_block *, char *);
mem_block *mem_str(char *);

char *itoa(int );
char *lltoa(long long );

integer *INTEGER_NODE();
integer *integer__init__(mem_block *);
integer *integer__cpy__(integer *);
integer *integer__neg__(integer *);
integer *integer__invert__(integer *);
mem_block *integer__str__(integer *);
integer *integer__inc__(integer *);
integer *integer__dec__(integer *);
int integer__eq__(integer *, integer *);
int integer__gt__(integer *, integer *);
int integer__lt__(integer *, integer *);
int integer__ge__(integer *, integer *);
int integer__le__(integer *, integer *);
integer *integer__add__(integer *, integer *);
integer *integer__sub__(integer *, integer *);
integer *integer__mkempt__(int size);
integer *integer__node__mul__(integer *, integer *);
integer *integer__mul__(integer *, integer *);
integer *integer__div__(integer *, integer *);
integer *integer__mod__(integer *, integer *);
integer *integer__pow__(integer *, integer *);
integer *integer__lshift__(integer *, integer *);
integer *integer__rshift__(integer *, integer *);
integer *integer__and__(integer *, integer *);
integer *integer__xor__(integer *, integer *);
integer *integer__or__(integer *, integer *);

#endif /* CPYTER_H */