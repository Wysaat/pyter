#ifndef CPYTER_H
#define CPYTER_H

#define ITEMSIZE 1024
#define STRINGSIZE 10
#define MEM_BLOCK_SZ 10

#define match(x, y) !strcmp(x, y)

enum types { int_expr_t, str_expr_t,
             parenth_form_t,
             power_t, u_expr_t, b_expr_t, comparison_t, 
             not_test_t, conditional_expression_t,
             expression_list_t, 
             pyint_t, pystr_t, pybool_t,
             pytuple_t, };

typedef struct list list;

typedef struct int_expr {
    int type;
    char value[ITEMSIZE];
} int_expr;

typedef struct str_expr {
    int type;
    char value[ITEMSIZE];
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
    char op[10];
    void *left;
    void *right;
} b_expr;

typedef struct comparison {
    int type;
    list *comparisons;
} comparison;

typedef struct not_test {
    int type;
    char op[10];
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

typedef struct list {
    void *content;
    struct list *next;
} list;

typedef struct pyint {
    int type;
    int value;
} pyint;

typedef struct pystr {
    int type;
    char value[ITEMSIZE];
} pystr;

typedef struct pybool {
    int type;
    int value;
} pybool;

typedef struct mem_block {
    char mem[MEM_BLOCK_SZ];
    struct mem_block *prev;
    struct mem_block *next;
} mem_block;

void list_append(list *, void *);
void list_add(list *, list *);

void *B_EXPR(void *, char *, void *);
void *PYINT(int );
void *PYSTR(char *);
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

char *last_item();
char *pop_item();
void *parse_u_expr();
void *parse_expression();
list *pa_exprs(char *);

pystr *str__mul__(pystr *, pyint *);
pystr *str__add__(pystr *, pystr *);

void mem_print(mem_block *block);
mem_block *mem_head();
char mem_subscription(mem_block *, int );
void mem_ncpy_out(char *, mem_block *, int, int);
int mem_ncmp(char *, mem_block *, int, int);

#endif /* CPYTER_H */