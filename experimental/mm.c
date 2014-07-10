typedef struct identifier {
    int type;
    int ref;
    char *value
} identifier;

void identifier_del(identifier *ptr) {
    free(ptr);
}

void identifier_ref(identifier *ptr) {
    ++ptr->ref;
}

int identifier_nref(identifier *ptr) {
    return --ptr->ref;
}

typedef struct int_expr {
    int type;
    int ref;
    char *value;
} int_expr;

void int_expr_del(int_expr *ptr) {
    free(ptr);
}

void int_expr_ref(int_expr *ptr) {
    ++ptr->ref;
}

int int_expr_nref(int_expr *ptr) {
    return --ptr->ref;
}

typedef struct bool_expr {
    int type;
    int value;
    int ref;
} bool_expr;

void bool_expr_del(bool_expr *ptr) {
    free(ptr);
}

void bool_expr_ref(bool_expr *ptr) {
    ++ptr->ref;
}

int bool_expr_nref(bool_expr *ptr) {
    return --ptr->ref;
}

typedef struct float_expr {
    int type;
    char *value;
    int ref;
} float_expr;

void float_expr_del(float_expr *ptr) {
    free(ptr);
}

void float_expr_ref(float_expr *ptr) {
    ++ptr->ref;
}

int float_expr_nref(float_expr *ptr) {
    return --ptr->ref;
}

typedef struct imag_expr {
    int type;
    char *value;
    int ref;
} imag_expr;

void imag_expr_del(imag_expr *ptr) {
    free(ptr);
}

void imag_expr_ref(imag_expr *ptr) {
    ++ptr->ref;
}

int imag_expr_nref(imag_expr *ptr) {
    return --ptr->ref;
}

typedef struct str_expr {
    int type;
    char *value;
    int ref;
} str_expr;

void str_expr_del(str_expr *ptr) {
    free(ptr);
}

void imag_expr_ref(imag_expr *ptr) {
    ++ptr->ref;
}

int imag_expr_nref(imag_expr *ptr) {
    return --ptr->ref;
}

typedef struct set_expr {
    int type;
    int ref;
    list *expr_head;
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} set_expr;

void set_expr_del(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void set_expr_ref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        ref(lptr->content);
    }
    ++ptr->ref;
}

int set_expr_nref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    return --ptr->ref;
}

typedef struct dict_expr {
    int type;
    int ref;
    list *expr_head;
    list *expr_head2;
    list *ptr;  /* for yield */
    list *ptr2;  /* for yield */
} dict_expr;

void dict_expr_del(dict_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    for (lptr = ptr->expr_head2; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void dict_expr_ref(dict_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        ref(lptr->content);
    }
    for (lptr = ptr->expr_head2; lptr; lptr = lptr->next) {
        ref(lptr->content);
    }
    ++ptr->ref;
}

int dict_expr_nref(dict_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        nref(lptr->content);
    }
    for (lptr = ptr->expr_head2; lptr; lptr = lptr->next) {
        nref(lptr->content);
    }
    return --ptr->ref;
}

typedef struct parenth_form {
    int type;
    int ref;
    list *expr_head;
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} parenth_form;

void parenth_form_del(parenth_form *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void parenth_form_ref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        ref(lptr->content);
    }
    ++ptr->ref;
}

int parenth_form_nref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        nref(lptr->content);
    }
    return --ptr->ref;
}

typedef struct generator {
    int type;
    int ref;
    suite *_suite;
} generator;

void generator_del(generator *ptr) {
    if (!nref(ptr->_suite))
        del(ptr->_suite);
    free(ptr);
}

void generator_ref(generator *ptr) {
    ref(ptr->_suite);
    ++ptr->ref;
}

int generator_nref(generator *ptr) {
    nref(ptr->_suite);
    return --ptr->ref;
}

typedef struct list_expr {
    int type;
    int ref;
    list *expr_head;
    list *ptr;  /* for yield */
    list *values;  /* for yield */
} list_expr;

void list_expr_del(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void list_expr_ref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        ref(lptr->content);
    }
    ++ptr->ref;
}

int list_expr_nref(set_expr *ptr) {
    list *lptr;
    for (lptr = ptr->expr_head; lptr; lptr = lptr->next) {
        nref(lptr->content);
    }
    return --ptr->ref;
}

typedef struct list_comprehension {
    int type;
    int ref;
    suite *_suite;
} list_comprehension;

void list_comprehension_del(list_comprehension *ptr) {
    if (!nref(ptr->_suite))
        del(ptr->_suite);
    free(ptr);
}

void list_comprehension_ref(list_comprehension *ptr) {
    ref(ptr->_suite);
    ++ptr->ref;
}

int list_comprehension_nref(list_comprehension *ptr) {
    nref(ptr->_suite);
    return --ptr->ref;
}

typedef struct yield_atom {
    int type;
    int ref;
    void *expressions;
    int yielded;
} yield_atom;

void yield_atom_del(yield_atom *ptr) {
    if (!nref(ptr->expressions))
        del(ptr->expressions);
    free(ptr);
}

void yield_atom_ref(yield_atom *ptr) {
    ref(ptr->expressions);
    ++ptr->ref;
}

void yield_atom_nref(yield_atom *ptr) {
    nref(ptr->expressions);
    return --ptr->ref;
}

typedef struct attributeref {
    int type;
    void *primary;
    identifier *id;
} attributeref;

void attributeref_del(attributeref *ptr) {
    if (!nref(ptr->primary))
        del(ptr->primary);
    if (!nref(ptr->id))
        del(ptr->id);
    free(ptr);
}

void attributeref_ref(attributeref *ptr) {
    ref(ptr->primary)
    ptr->id->ref++;
    ptr->ref++;
}

void attributeref_nref(attributeref *ptr) {
    nref(ptr->primary);
    ptr->id->ref--;
    ptr->ref--;
}

typedef struct slice_expr {
    int type;
    void *start;
    void *stop;
    void *step;
    void *start_val; /* for yield */
    void *stop_val;  /* for yield */
    void *step_val;  /* for yield */
} slice_expr;

void slice_expr_del(slice_expr *ptr) {
    if (!nref(ptr->step))
        del(ptr->stop);
    if (!nref(ptr->stop))
        del(ptr->stop);
    if (!nref(ptr->step))
        del(ptr->step);
    free(ptr);
}

void slice_expr_ref(slice_expr *ptr) {
    ref(ptr->start);
    ref(ptr->stop);
    ref(ptr->step);
    ptr->ref++;
}

void slice_expr_nref(slice_expr *ptr) {
    nref(ptr->start);
    nref(ptr->stop);
    nref(ptr->step);
    ptr->ref--;
}

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

void pyint_del(pyint *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pyint_ref(pyint *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pyint_nref(pyint *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}

struct pybool {
    int type;
    int ref;
    pyclass *class;
    int value;
};

void pybool_del(pybool *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pybool_ref(pybool *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pybool_nref(pybool *ptr) {
    nref(ptr->class);
    --ptr->ref;
}

struct pyfloat {
    int type;
    int ref;
    pyclass *class;
    double value;
};

void pyfloat_del(pyfloat *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pyfloat_ref(pyfloat *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pyfloat_nref(pyfloat *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}

struct pycomplex {
    int type;
    int ref;
    pyclass *class;
    pyfloat *real;
    pyfloat *imag;
};

void pycomplex_del(pycomplex *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    if (!nref(ptr->real))
        del(ptr->real);
    if (!nref(ptr->imag))
        del(ptr->imag);
    free(ptr);
}

void pycomplex_ref(pycomplex *ptr) {
    ref(ptr->class);
    ref(ptr->real);
    ref(ptr->imag);
    ++ptr->ref;
}

int pycomplex_nref(pycomplex *ptr) {
    nref(ptr->class);
    nref(ptr->real);
    nref(ptr->imag);
    return --ptr->ref;
}

struct pystr {
    int type;
    int ref;
    pyclass *class;
    char *value;
};

void pystr_del(pystr *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pystr_ref(pystr *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pystr_nref(pystr *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}

struct pylist {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

void pylist_del(pylist *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void pylist_ref(pylist *ptr) {
    ref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        ref(lptr->content);
    ++ptr->ref;
}

int pylist_nref(pylist *ptr) {
    nref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        nref(lptr->content);
    return --ptr->ref;
}

struct pytuple {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

void pytuple_del(pytuple *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void pytuple_ref(pytuple *ptr) {
    ref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        ref(lptr->content);
    ++ptr->ref;
}

int pytuple_nref(pytuple *ptr) {
    nref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        nref(lptr->content);
    return --ptr->ref;
}

struct pyset {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

void pyset_del(pyset *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void pyset_ref(pyset *ptr) {
    ref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        ref(lptr->content);
    ++ptr->ref;
}

int pyset_nref(pyset *ptr) {
    nref(ptr->class);
    list *lptr;
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        nref(lptr->content);
    return --ptr->ref;
}

struct pydict {
    int type;
    int ref;
    pyclass *class;
    list *keys;
    list *values;
};

void pydict_del(pydict *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    list *lptr;
    for (lptr = ptr->keys; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    for (lptr = ptr->values; lptr; lptr = lptr->next) {
        if (!nref(lptr->content))
            del(lptr->content);
    }
    free(ptr);
}

void pydict_ref(pydict *ptr) {
    ref(ptr->class);
    list *lptr;
    for (lptr = ptr->keys; lptr; lptr = lptr->next)
        ref(lptr->content);
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        ref(lptr->content);
    ++ptr->ref;
}

int pydict_nref(pydict *ptr) {
    nref(ptr->class);
    list *lptr;
    for (lptr = ptr->keys; lptr; lptr = lptr->next)
        nref(lptr->content);
    for (lptr = ptr->values; lptr; lptr = lptr->next)
        nref(lptr->content);
    return --ptr->ref;
}

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

void pyfunction_del(pyfunction *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    if (!nref(ptr->assign_values))
        del(ptr->assign_values);
    free(ptr);
}

void pyfunction_ref(pyfunction *ptr) {
    ref(ptr->class);
    ref(ptr->assign_values);
    ++ptr->ref;
}

int pyfunction_nref(pyfunction *ptr) {
    nref(ptr->class);
    nref(ptr->assign_values);
    return --ptr->ref;
}

struct pyclass {
    int type;
    int ref;
    pyclass *class;
    char *id;
    environment *env;
    list *inheritance;  // can be zero
};

void pyclass_del(pyclass *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    if (ptr->inheritance) {
        list *lptr;
        for (lptr = ptr->inheritance; lptr; lptr = lptr->next) {
            if (!nref(lptr->content))
                del(lptr->content);
        }
    }
    free(ptr);
}

void pyclass_ref(pyclass *ptr) {
    ref(ptr->class);
    if (ptr->inheritance) {
        list *lptr;
        for (lptr = ptr->inheritance; lptr; lptr = lptr->next) {
            ref(lptr->content);
        }
    }
    ++ptr->ref;
}

int pyclass_nref(pyclass *ptr) {
    nref(ptr->class);
    if (ptr->inheritance) {
        list *lptr;
        for (lptr = ptr->inheritance; lptr; lptr = lptr->next) {
            nref(lptr->content);
        }
    }
    return --ptr->ref;
}

struct pymodule {
    int type;
    int ref;
    pyclass *class;
    char *name;
    environment *env;
};

void pymodule_del(pymodule *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pymodule_ref(pymodule *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pymodule_nref(pymodule *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}

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

void pyNone_del(pyNone *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pyNone_ref(pyNone *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pyNone_nref(pyNone *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}

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

void pyrange_del(pyrange *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    if (!nref(ptr->start))
        del(ptr->start);
    if (!nref(ptr->stop))
        del(ptr->stop);
    if (!nref(ptr->step))
        del(ptr->step);
    free(ptr);
}

void pyrange_ref(pyrange *ptr) {
    ref(ptr->class);
    ref(ptr->start);
    ref(ptr->stop);
    ref(ptr->step);
    ++ptr->ref;
}

int pyrange_nref(pyrange *ptr) {
    nref(ptr->class);
    nref(ptr->start);
    nref(ptr->stop);
    nref(ptr->step);
    return --ptr->ref;
}

struct pybuiltin_function {
    int type;
    int ref;
    pyclass *class;
    char *id;
    void *(* func)();
    void *bound;
};

void pybuiltin_function_del(pybuiltin_function *ptr) {
    if (!nref(ptr->class))
        del(ptr->class);
    free(ptr);
}

void pybuiltin_function_ref(pybuiltin_function *ptr) {
    ref(ptr->class);
    ++ptr->ref;
}

int pybuiltin_function_nref(pybuiltin_function *ptr) {
    nref(ptr->class);
    return --ptr->ref;
}