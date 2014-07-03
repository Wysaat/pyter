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

/******************************************************/

typedef struct expression_stmt {
    int type;
    void *expression_list;
} expression_stmt;

typedef struct assignment_stmt {
    int type;
    list *targets_list;
    void *expressions;
} assignment_stmt;

typedef struct return_stmt {
    int type;
    void *expressions;
} return_stmt;

typedef struct yield_stmt {
    int type;
    void *expressions;
    int yielded;
} yield_stmt;

typedef struct break_stmt {
    int type;
} break_stmt;

typedef struct continue_stmt {
    int type;
} continue_stmt;

typedef struct pass_stmt {
    int type;
} pass_stmt;

typedef struct import_stmt {
    int type;
    list *module_name_list;
    list *as_name_list;
} import_stmt;

typedef struct import_stmt2 {
    int type;
    char *module_name;
    list *var_name_list;
    list *as_name_list;
} import_stmt2;

typedef struct del_stmt {
    int type;
    list *target_list;
} del_stmt;

typedef struct stmt_list {
    int type;
    list *stmts;
    list *ptr;  /* for yield */
} stmt_list;

typedef struct if_stmt {
    int type;
    list *condition_list;
    list *suite_list;
    list *ptr1, *ptr2;  /* for yield */
} if_stmt;

typedef struct while_stmt {
    int type;
    void *condition;
    list *suite_list;
    void *cond_val;  /* for yield */
} while_stmt;

typedef struct for_stmt {
    int type;
    void *targets;
    void *expressions;
    list *suite_list;
    void *values_list; /* for yield */
    list *list_ptr;    /* for yield */
    pyrange *range;    /* for range in yield */
    pyint *index;      /* for range in yield */
} for_stmt;

typedef struct funcdef {
    int type;
    identifier *id;
    list *parameters;
    void *fsuite;
    int yield;
    expression_list *assign_target_list;  // can be 0
    expression_list *assign_expr_list;  // can be 0
} funcdef;

typedef struct classdef {
    int type;
    identifier *id;
    list *inheritance;  // can be 0
    void *_suite;
} classdef;

typedef struct suite {
    int type;
    list *stmts;
    list *ptr;  /* for yield */
} suite;

/******************************************************/

struct pyint {
    int type;
    int ref;
    pyclass *class;
    integer *value;
};

struct pybool {
    int type;
    int ref;
    pyclass *class;
    int value;
};

struct pyfloat {
    int type;
    int ref;
    pyclass *class;
    double value;
};

struct pycomplex {
    int type;
    int ref;
    pyclass *class;
    pyfloat *real;
    pyfloat *imag;
};

struct pystr {
    int type;
    int ref;
    pyclass *class;
    char *value;
};

struct pylist {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

struct pytuple {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

struct pyset {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

struct pydict {
    int type;
    int ref;
    pyclass *class;
    list *keys;
    list *values;
};

struct pyfunction {
    int type;
    int ref;
    pyclass *class;
    identifier *id;
    list *parameters;  // can be 0, or a list of identifiers
    void *fsuite;
    environment *env;
    void *bound;
    int yield;  /* for generators */
    expression_list *assign_target_list;  // can be 0
    pytuple *assign_values;  // can be 0
};

struct pyclass {
    int type;
    int ref;
    pyclass *class;
    char *id;
    environment *env;
    list *inheritance;  // can be zero
};

struct pymodule {
    int type;
    int ref;
    pyclass *class;
    char *name;
    environment *env;
};

struct pyslice {
    int type;
    int start;
    int stop;
    int step;
    int nostart; // e.g. a[::-1]
    int nostop; // e.g. a[2:] 
};

struct pyNone {
    int type;
    int ref;
    pyclass *class;
};

struct pyargument {
    int type;
    list *assign_target_list;  // can be 0
    list *assign_value_list;  // can be 0
    list *value_list;  // can be 0
};

struct pyrange {
    int type;
    int ref;
    pyclass *class;
    pyint *start;
    pyint *stop;
    pyint *step;
};

struct pybuiltin_function {
    int type;
    int ref;
    pyclass *class;
    char *id;
    void *(* func)();
    void *bound;
};
