#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cpyter.h"
#include "evaluate.h"
#include "environment.h"
#include "struct_info.h"

void *IDENTIFIER(char *token) {
    identifier *retptr = (identifier *)malloc(sizeof(identifier));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = identifier_t;
    if (token)
        retptr->value = strdup(token);
    else /* nameless identifier, for comprehensions */
        retptr->value = 0;
    return retptr;
}

/* op should be copied(use memory.c function mem_cpy) */
void *INT_EXPR(char *token) {
    int_expr *retptr = (int_expr *)malloc(sizeof(int_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = int_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *FLOAT_EXPR(char *token) {
    float_expr *retptr = (float_expr *)malloc(sizeof(float_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = float_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *IMAG_EXPR(char *token) {
    imag_expr *retptr = (imag_expr *)malloc(sizeof(imag_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = imag_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *STR_EXPR(char *token) {
    str_expr *retptr = (str_expr *)malloc(sizeof(str_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = str_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *PARENTH_FORM(list *expr_head) {
    parenth_form *retptr = (parenth_form *)malloc(sizeof(parenth_form));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = parenth_form_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *GENERATOR(suite *_suite) {
    generator *retptr = (generator *)malloc(sizeof(generator));
    retptr->type = generator_t;
    retptr->_suite = _suite;
    return retptr;
}

void *LIST_EXPR(list *expr_head) {
    list_expr *retptr = (list_expr *)malloc(sizeof(list_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = list_expr_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *SET_EXPR(list *expr_head) {
    set_expr *retptr = (set_expr *)malloc(sizeof(set_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = set_expr_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *DICT_EXPR(list *expr_head, list *expr_head2) {
    dict_expr *retptr = (dict_expr *)malloc(sizeof(dict_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = dict_expr_t;
    retptr->expr_head = expr_head;
    retptr->expr_head2 = expr_head2;
    return retptr;
}

void *LIST_COMPREHENSION(suite *_suite) {
    list_comprehension *retptr = (list_comprehension *)malloc(sizeof(list_comprehension));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = list_comprehension_t;
    retptr->_suite = _suite;
    return retptr;
}

void *YIELD_ATOM(void *expressions) {
    yield_atom *retptr = (yield_atom *)malloc(sizeof(yield_atom));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = yield_atom_t;
    retptr->expressions = expressions;
    retptr->yielded = 0;
}

void yield_atom_del(void *vptr) {
    yield_atom *ptr = (yield_atom *)vptr;
    del(ptr->expressions);
    free(ptr);
}

void *ATTRIBUTEREF(void *primary, identifier *id) {
    attributeref *retptr = (attributeref *)malloc(sizeof(attributeref));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = attributeref_t;
    retptr->primary = primary;
    retptr->id = id;
    return retptr;
}

void *SLICE_EXPR(void *start, void *stop, void *step) {
    slice_expr *retptr = (slice_expr *)malloc(sizeof(slice_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = slice_expr_t;
    retptr->start = start;
    retptr->stop = stop;
    retptr->step = step;
    return retptr;
}

void *SUBSC_EXPR(void *value) {
    subsc_expr *retptr = (subsc_expr *)malloc(sizeof(subsc_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = subsc_expr_t;
    retptr->value = value;
    return retptr;
}

void *SLICING(void *primary, slice_expr *slice) {
    slicing *retptr = (slicing *)malloc(sizeof(slicing));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = slicing_t;
    retptr->primary = primary;
    retptr->slice = slice;
    return retptr;
}

void *SUBSCRIPTION(void *primary, subsc_expr *subsc) {
    subscription *retptr = (subscription *)malloc(sizeof(subscription));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = subscription_t;
    retptr->primary = primary;
    retptr->subsc = subsc;
    return retptr;
}

void *CALL(void *primary, list *arguments) {
    call *retptr = (call *)malloc(sizeof(call));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = call_t;
    retptr->primary = primary;
    retptr->arguments = arguments;
    return retptr;
}

void *POWER(void *primary, void *u_expr) {
    power *retptr = (power *)malloc(sizeof(power));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = power_t;
    retptr->primary = primary;
    retptr->u_expr = u_expr;
    return retptr;
}

void *U_EXPR(char *op, void *expr) {
    u_expr *retptr = (u_expr *)malloc(sizeof(u_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = u_expr_t;
    retptr->op = strdup(op);
    retptr->expr = expr;
    return retptr;
}

void *B_EXPR(void *left, char *op, void *right) {
    b_expr *retptr = (b_expr *)malloc(sizeof(b_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = b_expr_t;
    retptr->op = strdup(op);
    retptr->left = left;
    retptr->right = right;
    return retptr;
}

void *NOT_TEST(void *expr) {
    not_test *retptr = (not_test *)malloc(sizeof(not_test));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = not_test_t;
    retptr->expr = expr;
    return retptr;
}

void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr) {
    conditional_expression *retptr = \
        (conditional_expression *)malloc(sizeof(conditional_expression));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = conditional_expression_t;
    retptr->or_test = or_test;
    retptr->or_test2 = or_test2;
    retptr->expr = expr;
    return retptr;
}

void *LAMBDA_EXPR(list *parameters, void *expr, expression_list *assign_target_list,
                      expression_list *assign_expr_list) {
    lambda_expr *retptr = (lambda_expr *)malloc(sizeof(lambda_expr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = lambda_expr_t;
    retptr->parameters = parameters;
    retptr->expr = expr;
    retptr->assign_target_list = assign_target_list;
    retptr->assign_expr_list = assign_expr_list;
    return retptr;
}

void *EXPRESSION_LIST(list *expr_head) {
    expression_list *retptr = (expression_list *)malloc(sizeof(expression_list));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = expression_list_t;
    retptr->expr_head = expr_head;
    retptr->value_list = 0;
    retptr->expr_ptr = 0;
    return retptr;
}

void *PYINT(integer *value) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = pyint_t;
    retptr->value = value;
    return retptr;
}

void *PYSTR(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = pystr_t;
    retptr->value = strdup(value);
    return retptr;
}

/* CAUTION env->val_dict could be an empty list */
void *identifierEvaluate(identifier *structure, environment *env) {
    list *ptr;
    environment *env_ptr;
    if (structure->value) {
        for (env_ptr = env; env_ptr; env_ptr = env_ptr->outer) {
            if (!list_is_empty(env_ptr->val_dict)) {
                for (ptr = env_ptr->val_dict; ptr; ptr = ptr->next) {
                    val_dict_entry *entry = (val_dict_entry *)ptr->content;
                    if (entry->id && !strcmp(entry->id, structure->value)) {
                        ref(entry->value);
                        return entry->value;
                    }
                }
            }
        }
    }
    else { /* nameless identifer, for comprehensions */
        for (env_ptr = env; env_ptr; env_ptr = env_ptr->outer) {
            if (!list_is_empty(env_ptr->val_dict)) {
                for (ptr = env_ptr->val_dict; ptr; ptr = ptr->next) {
                    val_dict_entry *entry = (val_dict_entry *)ptr->content;
                    if (!entry->id) {
                        ref(entry->value);
                        return entry->value;
                    }
                }
            }
        }
    }
}

void identifier_del(void *vptr) {
    identifier *ptr = (identifier *)vptr;
    free(ptr->value);
    free(ptr);
}

void *int_exprEvaluate(int_expr *structure) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__init__(structure->value);
    ref(retptr);
    return retptr;
}

void int_expr_del(void *vptr) {
    int_expr *ptr = (int_expr *)vptr;
    free(ptr->value);
    free(ptr);
}

void *float_exprEvaluate(float_expr *structure) {
    pyfloat *retptr = (pyfloat *)calloc(sizeof(pyfloat), 1);
    retptr->type = pyfloat_t;
    retptr->value = atof(structure->value);
    ref(retptr);
    return retptr;
}

void float_expr_del(void *vptr) {
    float_expr *ptr = (float_expr *)vptr;
    free(ptr->value);
    free(ptr);
}

void *imag_exprEvaluate(imag_expr *structure) {
    pycomplex *retptr = (pycomplex *)calloc(sizeof(pycomplex), 1);
    retptr->type = pycomplex_t;
    char *mag = strdup(structure->value);
    mag[strlen(mag)-1] = 0;
    retptr->real = pyfloat__init__();
    retptr->imag = float_exprEvaluate(FLOAT_EXPR(mag));
    free(mag);
    ref_inc(retptr->real);
    ref_inc(retptr);
    return retptr;
}

void imag_expr_del(void *vptr) {
    imag_expr *ptr = (imag_expr *)vptr;
    free(ptr->value);
    free(ptr);
}

void *str_exprEvaluate(str_expr *structure) {
    pystr *retptr = pystr_init2(strslice(structure->value, 1, -1, 1));
    ref(retptr);
    return retptr;
}

void str_expr_del(void *vptr) {
    str_expr *ptr = (str_expr *)vptr;
    free(ptr->value);
    free(ptr);
}

void *parenth_formEvaluate(parenth_form *structure, environment *env) {
    pytuple *retptr = (pytuple *)calloc(sizeof(pytuple), 1);
    retptr->type = pytuple_t;
    retptr->values = list_node();
    list *ptr;
    if (list_is_empty(structure->expr_head))
        return retptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        void *appendp = evaluate(ptr->content, env);
        list_append_content(retptr->values, appendp);
    }
    ref_inc(retptr);
    return retptr;
}

void parenth_form_del(void *vptr) {
    parenth_form *ptr = (parenth_form *)vptr;
    del(ptr->expr_head);
    free(ptr);
}

void *generatorEvaluate(generator *structure, environment *env) {
    return pygenerator_init(structure->_suite, environment_init(env), 0);
}

void generator_del(void *vptr) {
    free(vptr);
}

void *list_exprEvaluate(list_expr *structure, environment *env) {
    pylist *retptr = (pylist *)calloc(sizeof(pylist), 1);
    retptr->type = pylist_t;
    retptr->values = list_node();
    list *ptr;
    if (list_is_empty(structure->expr_head))
        return retptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        void *appendp = evaluate(ptr->content, env);
        list_append_content(retptr->values, appendp);
    }
    ref_inc(retptr);
    return retptr;
}

void list_expr_del(void *vptr) {
    list_expr *ptr = (list_expr *)vptr;
    del(ptr->expr_head);
    free(ptr);
}

void *list_comprehensionEvaluate(list_comprehension *structure, environment *env) {
    environment *local_env = environment_init(env);
    execute(structure->_suite, local_env, 0);
    list *ptr;
    for (ptr = local_env->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (!entry->id) {
            ref(entry->value);
            return entry->value;
        }
    }
}

void list_comprehension_del(void *vptr) {
    list_comprehension *ptr = (list_comprehension *)vptr;
    del(ptr->_suite);
    free(ptr);
}

void *set_exprEvaluate(set_expr *structure, environment *env) {
    pyset *retptr = (pyset *)malloc(sizeof(pyset));
    retptr->ref = 0;
    retptr->class = &set_class;
    retptr->type = pyset_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        void *val = evaluate(ptr->content, env);
        if (list_find(retptr->values, val) < 0)
            list_append_content(retptr->values, val);
        else
            del(val);
    }
    ref_inc(retptr);
    return retptr;
}

void set_expr_del(void *vptr) {
    set_expr *ptr = (set_expr *)vptr;
    del(ptr->expr_head);
    free(ptr);
}

void *dict_exprEvaluate(dict_expr *structure, environment *env) {
    pydict *retptr = (pydict *)malloc(sizeof(pydict));
    retptr->ref = 0;
    retptr->type = pydict_t;
    retptr->class = &dict_class;
    retptr->keys = list_node();
    retptr->values = list_node();
    list *ptr = structure->expr_head, *ptr2 = structure->expr_head2;
    void *key, *val;
    int pos;
    while (ptr) {
        key = evaluate(ptr->content, env);
        val = evaluate(ptr2->content, env);
        if ((pos = list_find(retptr->keys, key)) >= 0) {
            int ind;
            list *ptr = retptr->keys;
            for (ind = 0; ind < pos; ind++)
                ptr = ptr->next;
            del(ptr->content);
            ptr->content = val;
            del(key);
        }
        else {
            list_append_content(retptr->keys, key);
            list_append_content(retptr->values, val);
        }
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }
    ref_inc(retptr);
    return retptr;
}

void dict_expr_del(void *vptr) {
    dict_expr *ptr = (dict_expr *)vptr;
    del(ptr->expr_head);
    del(ptr->expr_head2);
    free(ptr);
}

void *attributerefEvaluate(attributeref *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    list *ptr, *val_dict;
    pyclass *class = get_class(primary_val);

    if (type(primary_val) == pyclass_t)
        val_dict = ((pyclass *)primary_val)->env->val_dict;
    else if (type(primary_val) == instance_t)
        val_dict = ((instance *)primary_val)->env->val_dict;
    else if (type(primary_val) == pymodule_t)
        val_dict = ((pymodule *)primary_val)->env->val_dict;
    else
        val_dict = list_node();

    if (!list_is_empty(val_dict)) {
        for (ptr = val_dict; ptr; ptr = ptr->next) {
            val_dict_entry *entry = (val_dict_entry *)ptr->content;
            if (!strcmp(entry->id, structure->id->value)) {
                if (type(entry->value) == pyfunction_t)
                    ((pyfunction *)entry->value)->bound = 0;
                else if (type(entry->value) == pybuiltin_function_t)
                    ((pybuiltin_function *)entry->value)->bound = 0;
                del(primary_val);
                void *retptr = entry->value;
                ref(retptr);
                return retptr;
            }
        }
    }

    if (type(primary_val) == pyclass_t && ((pyclass *)primary_val)->inheritance) {
        list *ptr2, *ptr3;
        for (ptr2 = ((pyclass *)primary_val)->inheritance; ptr2; ptr2 = ptr2->next) {
            pyclass *_class = (pyclass *)ptr2->content;
            if (!list_is_empty(_class->env->val_dict)) {
                for (ptr3 = _class->env->val_dict; ptr3; ptr3 = ptr3->next) {
                    val_dict_entry *entry = (val_dict_entry *)ptr3->content;
                    if (!strcmp(entry->id, structure->id->value)) {
                        if (type(entry->value) == pyfunction_t)
                            ((pyfunction *)entry->value)->bound = 0;
                        else if (type(entry->value) == pybuiltin_function_t)
                            ((pybuiltin_function *)entry->value)->bound = 0;
                        del(primary_val);
                        void *retptr = entry->value;
                        ref(retptr);
                        return retptr;
                    }
                }
            }
        }
    }
    void *attr = structure->id->value;
    void *retptr = __getattribute__(class, primary_val, attr);
    del(primary_val);
    ref(retptr);
    return retptr;
}

void attributeref_del(void *vptr) {
    attributeref *ptr = (attributeref *)vptr;
    del(ptr->primary);
    del(ptr->id);
}

void *slice_exprEvaluate(slice_expr *structure, environment *env) {
    pyslice *retptr = (pyslice *)calloc(sizeof(pyslice), 1);
    retptr->type = pyslice_t;
    if (structure->start) {
        void *startp = evaluate(structure->start, env);
        retptr->start = pyint_to_int(startp);
        del(startp);
        retptr->nostart = 0;
    }
    else {
        retptr->start = 0;
        retptr->nostart = 1;
    }
    if (structure->stop) {
        void *stopp = evaluate(structure->stop, env);
        retptr->stop = pyint_to_int(stopp);
        del(stopp);
        retptr->nostop = 0;
    }
    else {
        retptr->stop = 0;
        retptr->nostop = 1;
    }
    if (structure->step) {
        void *stepp = evaluate(structure->step, env);
        retptr->step = pyint_to_int(stepp);
        del(stepp);
    }
    else
        retptr->step = 1;
    return retptr;
}

void slice_expr_del(void *vptr) {
    slice_expr *ptr = (slice_expr *)vptr;
    if (ptr->start)
        del(ptr->start);
    if (ptr->stop)
        del(ptr->stop);
    if (ptr->step)
        del(ptr->step);
    free(ptr);
}

void *slicingEvaluate(slicing *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *slice_val = evaluate(structure->slice, env);
    void *retptr = __getitem__(primary_val, slice_val);
    del(primary_val);
    del(slice_val);
    ref(retptr);
    return retptr;
}

void slicing_del(void *vptr) {
    slicing *ptr = (slicing *)vptr;
    del(ptr->primary);
    del(ptr->slice);
    free(ptr);
}

void *subscriptionEvaluate(subscription *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *subsc_val = evaluate(structure->subsc->value, env);
    void *retptr = __getitem__(primary_val, subsc_val);
    del(primary_val);
    del(subsc_val);
    ref(retptr);
    return retptr;
}

void subsc_expr_del(void *vptr) {
    subsc_expr *ptr = (subsc_expr *)vptr;
    del(ptr->value);
    free(ptr);
}

void subscription_del(void *vptr) {
    subscription *ptr = (subscription *)vptr;
    del(ptr->primary);
    del(ptr->subsc);
    free(ptr);
}

void *callEvaluate(call *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    list *target_list, *assign_expr_list, *expressions;
    void *retptr;
    if (structure->arguments) {
        target_list = structure->arguments->content,
        assign_expr_list = structure->arguments->next->content,
        expressions = structure->arguments->next->next->content;
        list *assign_target_list, *assign_value_list, *value_list, *ptr;
        if (!list_is_empty(target_list)) {
            assign_target_list = target_list;
            assign_value_list = list_node();
            for (ptr = assign_expr_list; ptr; ptr = ptr->next) {
                void *appendp = evaluate(ptr->content, env);
                list_append_content(assign_value_list, appendp);
            }
        }
        else {
            assign_target_list = 0;
            assign_value_list = 0;
        }
        if (!list_is_empty(expressions)) {
            value_list = list_node();
            for (ptr = expressions; ptr; ptr = ptr->next) {
                void *appendp = evaluate(ptr->content, env);
                list_append_content(value_list, appendp);
            }
        }
        else
            value_list = 0;
        pyargument *argument = (pyargument *)malloc(sizeof(pyargument));
        argument->type = __pyargument_t;
        argument->assign_target_list = assign_target_list;
        argument->assign_value_list = assign_value_list;
        argument->value_list = value_list;

        retptr = __call__(primary_val, argument);
        del(primary_val);
        del(argument);
    }
    else {
        retptr = __call__(primary_val, 0);
        del(primary_val);
    }

    return retptr;
}

void call_del(void *vptr) {
    call *ptr = (call *)vptr;
    del(ptr->primary);
    if (ptr->arguments) {
        del(ptr->arguments->next->content);
        del(ptr->arguments->next->next->content);
        free(ptr->arguments->next->next);
        free(ptr->arguments->next);
        free(ptr->arguments);
    }
    free(ptr);
}

void *powerEvaluate(power *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *u_expr_val = evaluate(structure->u_expr, env);
    void *retptr = __pow__(primary_val, u_expr_val);
    del(primary_val);
    del(u_expr_val);
    ref(retptr);
    return retptr;
}

void power_del(void *vptr) {
    power *ptr = (power *)vptr;
    del(ptr->primary);
    del(ptr->u_expr);
    free(ptr);
}

void *u_exprEvaluate(u_expr *structure, environment *env) {
    void *expr_val = evaluate(structure->expr, env);
    void *retptr;
    if (!strcmp(structure->op, "+")) {
        if (type(expr_val) == pyint_t)
            retptr = expr_val;
        else if (type(expr_val) == pyfloat_t)
            retptr = expr_val;
        else if (type(expr_val) == pycomplex_t)
            retptr = expr_val;
    }
    else if (!strcmp(structure->op, "-")) {
        if (type(expr_val) == pyint_t)
            retptr = pyint__neg__((pyint *)expr_val);
        else if (type(expr_val) == pyfloat_t)
            retptr = pyfloat__neg__((pyfloat *)expr_val);
        else if (type(expr_val) == pycomplex_t)
            retptr = pycomplex__neg__((pycomplex *)expr_val);
    }
    else if (!strcmp(structure->op, "~")) {
        retptr = pyint__invert__((pyint *)expr_val);
    }
    del(expr_val);
    ref(retptr);
    return retptr;
}

void u_expr_del(void *vptr) {
    u_expr *ptr = (u_expr *)vptr;
    free(ptr->op);
    del(ptr->expr);
    free(ptr);
}

void *b_exprEvaluate(b_expr *structure, environment *env) {
    int *left_val = evaluate(structure->left, env);
    int *right_val = evaluate(structure->right, env);
    integer *zero = INTEGER_NODE();
    void *retptr;
    if (!strcmp(structure->op, "*"))
        retptr = __mul__(left_val, right_val);
    else if (!strcmp(structure->op, "/"))
        retptr = __div__(left_val, right_val);
    else if (!strcmp(structure->op, "//"))
        retptr = __rfloordiv__(left_val, right_val);
    else if (!strcmp(structure->op, "+"))
        retptr = __add__(left_val, right_val);
    else if (!strcmp(structure->op, "-"))
        retptr =  __sub__(left_val, right_val);
    else if (!strcmp(structure->op, "in")) {
        if (type(right_val) == pylist_t) {
            if (list_find(((pylist *)right_val)->values, left_val) >= 0)
                retptr = PYBOOL(1);
            else
                retptr = PYBOOL(0);
        }
        else if (type(right_val) == pyrange_t) {
            pyrange *range = (pyrange *)right_val;
            if (is_true(pyint__ge__(left_val, range->stop)) || is_true(pyint__lt__(left_val, range->start)))
                retptr = PYBOOL(0);
            else if (!is_true(__bool__(pyint__mod__(pyint__sub__(left_val, range->start), range->step))))
                retptr = PYBOOL(1);
            else
                retptr = PYBOOL(0);
        }
    }

    else if (*left_val == pyint_t && *right_val == pyint_t) {
        integer *left = ((pyint *)left_val)->value;
        integer *right = ((pyint *)right_val)->value;
        if (!strcmp(structure->op, "%"))
            retptr = pyint__mod__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "<<"))
            retptr = pyint__lshift__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, ">>"))
            retptr = pyint__rshift__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "&"))
            retptr = PYINT(integer__and__(left, right));
        else if (!strcmp(structure->op, "^"))
            retptr = PYINT(integer__xor__(left, right));
        else if (!strcmp(structure->op, "|"))
            retptr = PYINT(integer__or__(left, right));
        else if (!strcmp(structure->op, "<"))
            retptr = pyint__lt__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, ">"))
            retptr = PYBOOL(integer__gt__(left, right));
        else if (!strcmp(structure->op, "=="))
            retptr = PYBOOL(integer__eq__(left, right));
        else if (!strcmp(structure->op, "<="))
            retptr = PYBOOL(integer__le__(left, right));
        else if (!strcmp(structure->op, ">="))
            retptr = PYBOOL(integer__ge__(left, right));
        else if (!strcmp(structure->op, "<>"))
            retptr = PYBOOL(!integer__eq__(left, right));
        else if (!strcmp(structure->op, "!="))
            retptr = PYBOOL(!integer__eq__(left, right));
        else if (!strcmp(structure->op, "and")) {
            if (!integer__eq__(left, zero) && !integer__eq__(right, zero))
                retptr = PYINT(right);
            else
                retptr = PYINT(zero);
        }
        else if (!strcmp(structure->op, "or")) {
            if (!integer__eq__(left, zero))
                retptr = PYINT(left);
            else if (!integer__eq__(right, zero))
                retptr = PYINT(right);
            else
                retptr = PYINT(0);
        }
    }

    del(left_val);
    del(right_val);
    ref(retptr);

    return retptr;
}

void b_expr_del(void *vptr) {
    b_expr *ptr = (b_expr *)vptr;
    free(ptr->op);
    del(ptr->left);
    del(ptr->right);
    free(ptr);
}

void *not_testEvaluate(not_test *structure, environment *env) {
    integer *zero = INTEGER_NODE();
    int *expr_val = evaluate(structure->expr, env);
    void *retptr;
    if (*expr_val == pyint_t) {
        pyint *val = (pyint *)expr_val;
        if (integer__eq__(val->value, zero))
            retptr = PYBOOL(1);
        else
            retptr = PYBOOL(0);
    }
    else if (*expr_val == pystr_t) {
        retptr = PYBOOL(0);
    }
    else if (*expr_val == pybool_t) {
        if (((pybool *)expr_val)->value)
            retptr = PYBOOL(0);
        else
            retptr = PYBOOL(1);
    }
    del(expr_val);
    ref(retptr);
    return retptr;
}

void not_test_del(void *vptr) {
    not_test *ptr = (not_test *)vptr;
    del(ptr->expr);
    free(ptr);
}

void *comparisonEvaluate(comparison *structure, environment *env) {
    list *ptr;
    int *val;
    void *retptr;
    for (ptr = structure->comparisons; ptr != 0; ptr = ptr->next) {
        val = evaluate(ptr->content, env);
        if (type(val) == pyint_t && integer__eq__(((pyint *)val)->value, INTEGER_NODE())) {
            del(val);
            retptr = PYBOOL(0);
            ref(retptr);
            return retptr;
        }
        else if (type(val) == pybool_t && ((pybool *)val)->value == 0) {
            del(val);
            retptr = PYBOOL(0);
            ref(retptr);
            return retptr;
        }
    }
    retptr = PYBOOL(1);
    ref(retptr);
    return retptr;
}

void comparison_del(void *vptr) {
    comparison *ptr = (comparison *)vptr;
    del(ptr->comparisons);
    free(ptr);
}

void *conditional_expressionEvaluate(conditional_expression *structure, environment *env) {
    void *retptr, *condptr;
    integer *zero = INTEGER_NODE();
    retptr = evaluate(structure->or_test, env);
    condptr = evaluate(structure->or_test2, env);
    if (*(int *)condptr == pyint_t && !integer__eq__(((pyint *)condptr)->value, zero)) {
        del(condptr);
        return retptr;
    }
    else if (*(int *)condptr == pybool_t && ((pybool *)condptr)->value != 0) {
        del(condptr);
        return retptr;
    }
    else if (*(int *)condptr == pystr_t) {
        del(condptr);
        return retptr;
    }
    del(condptr);
    del(retptr);
    retptr = evaluate(structure->expr, env);
    return retptr;
}

void conditional_expression_del(void *vptr) {
    conditional_expression *ptr = (conditional_expression *)vptr;
    del(ptr->or_test);
    del(ptr->or_test2);
    del(ptr->expr);
    free(ptr);
}

void *lambda_exprEvaluate(lambda_expr *structure, environment *env) {
    pyfunction *retptr = (pyfunction *)malloc(sizeof(pyfunction));
    memset(retptr, 0, sizeof(pyfunction));
    retptr->type = pyfunction_t;
    retptr->id = 0;
    retptr->parameters = structure->parameters;
    retptr->fsuite = RETURN_STMT(structure->expr);
    retptr->env = env;
    retptr->assign_target_list = structure->assign_target_list;
    if (structure->assign_target_list)
        retptr->assign_values = evaluate(structure->assign_expr_list, env);
    else
        retptr->assign_values = 0;
    return retptr;
}

void lambda_expr_del(void *vptr) {
    lambda_expr *ptr = (lambda_expr *)vptr;
    free(ptr);
}

void *expression_listEvaluate(expression_list *structure, environment *env) {
    list *expr_ptr;
    list *value_list = list_node();
    for (expr_ptr = structure->expr_head; expr_ptr; expr_ptr = expr_ptr->next) {
        void *appendp = evaluate(expr_ptr->content, env);
        list_append_content(value_list, appendp);
    }
    pytuple *retptr = pytuple__init__();
    retptr->values = value_list;
    ref_inc(retptr);
    return retptr;
}

void expression_list_del(void *vptr) {
    expression_list *ptr = (expression_list *)vptr;
    free(ptr->expr_head);
    free(ptr);
}

void *evaluate(void *structure, environment *env) {
    void *retptr;
    if (!structure)
        retptr = pyNone_init();
    else switch (*(int *)structure) {
        case identifier_t:
            retptr = identifierEvaluate((identifier *)structure, env);
            break;
        case int_expr_t:
            retptr = int_exprEvaluate((int_expr *)structure);
            break;
        case float_expr_t:
            retptr = float_exprEvaluate((float_expr *)structure);
            break;
        case imag_expr_t:
            retptr = imag_exprEvaluate((imag_expr *)structure);
            break;
        case str_expr_t:
            retptr = str_exprEvaluate((str_expr *)structure);
            break;
        case parenth_form_t:
            retptr = parenth_formEvaluate((parenth_form *)structure, env);
            break;
        case generator_t:
            retptr = generatorEvaluate((generator *)structure, env);
            break;
        case list_expr_t:
            retptr = list_exprEvaluate((list_expr *)structure, env);
            break;
        case list_comprehension_t:
            retptr = list_comprehensionEvaluate((list_comprehension *)structure, env);
            break;
        case set_expr_t:
            retptr = set_exprEvaluate((set_expr *)structure, env);
            break;
        case dict_expr_t:
            retptr = dict_exprEvaluate((dict_expr *)structure, env);
            break;
        case attributeref_t:
            retptr = attributerefEvaluate((attributeref *)structure, env);
            break;
        case slice_expr_t:
            retptr = slice_exprEvaluate((slice_expr *)structure, env);
            break;
        case slicing_t:
            retptr = slicingEvaluate((slicing *)structure, env);
            break;
        case subscription_t:
            retptr = subscriptionEvaluate((subscription *)structure, env);
            break;
        case call_t:
            retptr = callEvaluate((call *)structure, env);
            break;
        case power_t:
            retptr = powerEvaluate((power *)structure, env);
            break;
        case u_expr_t:
            retptr = u_exprEvaluate((u_expr *)structure, env);
            break;
        case b_expr_t:
            retptr = b_exprEvaluate((b_expr *)structure, env);
            break;
        case comparison_t:
            retptr = comparisonEvaluate((comparison *)structure, env);
            break;
        case not_test_t:
            retptr = not_testEvaluate((not_test *)structure, env);
            break;
        case conditional_expression_t:
            retptr = conditional_expressionEvaluate((conditional_expression *)structure, env);
            break;
        case lambda_expr_t:
            retptr = lambda_exprEvaluate((lambda_expr *)structure, env);
            break;
        case expression_list_t:
            retptr = expression_listEvaluate((expression_list *)structure, env);
            break;
        default:
            printf("[evaluate]: can't evaluate this structure..\n");
            break;
    }
    return retptr;
}

/* no '\n' append */
void print_nnl(void *structure) {
    list *ptr, *ptr2;
    switch (*(int *)structure) {
        case pyint_t:
            printf("%s", integer__str__(((pyint *)structure)->value));
            break;
        case pyfloat_t:
            printf("%le", ((pyfloat *)structure)->value);
            break;
        case pycomplex_t:
            printf("(");
            print_nnl(((pycomplex *)structure)->real);
            printf("+");
            print_nnl(((pycomplex *)structure)->imag);
            printf("j)");
            break;
        case pystr_t:
            pystr_print_nnl((pystr *)structure);
            break;
        case pybool_t:
            if (((pybool *)structure)->value)
                printf("True");
            else
                printf("False");
            break;
        case pytuple_t:
            printf("(");
            if (!list_is_empty(((pytuple *)structure)->values)) {
                ptr = ((pytuple *)structure)->values;
                if (!ptr->next) {
                    print_nnl(ptr->content);
                    printf(",");
                }
                else {
                    for (ptr = ((pytuple *)structure)->values; ptr; ptr = ptr->next) {
                        print_nnl(ptr->content);
                        if (ptr->next)
                            printf(", ");
                    }
                }
            }
            printf(")");
            break;
        case pylist_t:
            printf("[");
            if (!list_is_empty(((pylist *)structure)->values))
                for (ptr = ((pylist *)structure)->values; ptr; ptr = ptr->next) {
                    if (ptr->content == structure)
                        printf("[...]");
                    else
                        print_nnl(ptr->content);
                    if (ptr->next)
                        printf(", ");
                }
            printf("]");
            break;
        case pyset_t:
            printf("{");
            for (ptr = ((pyset *)structure)->values; ptr; ptr = ptr->next) {
                print_nnl(ptr->content);
                if (ptr->next)
                    printf(", ");
            }
            printf("}");
            break;
        case pydict_t:
            printf("{");
            for (ptr = ((pydict *)structure)->keys, ptr2 = ((pydict *)structure)->values; 
                    ptr; ptr = ptr->next, ptr2 = ptr2->next) {
                print_nnl(ptr->content);
                printf(": ");
                print_nnl(ptr2->content);
                if (ptr->next)
                    printf(", ");
            }
            printf("}");
            break;
        case pyfunction_t:
            if (((pyfunction *)structure)->id)
                printf("<function %s at %p>", ((pyfunction *)structure)->id->value, &structure);
            else
                printf("<function %s at %p>", "<lambda>", &structure);
            break;
        case pyNone_t:
            printf("None");
            break;
        case pybuiltin_function_t:
            printf("<built-in function %s>", ((pybuiltin_function *)structure)->id);
            break;
        case pyclass_t:
            printf("<class '%s'>", ((pyclass *)structure)->id);
            break;
        case pyrange_t:
            printf("%s", pystr_to_str(str(structure)));
            break;
        case pygenerator_t:
            if (((pygenerator *)structure)->id)
                printf("<generator object %s at %p>", ((pygenerator *)structure)->id, structure);
            else
                printf("<generator object %s at %p>", "<genexpr>", structure);
            break;
    }
}

/* '\n' append */
void print(void *structure) {
    print_nnl(structure);
    printf("\n");
}

int type(void *val) {
    return *(int *)val;
}

int get_ref(void *val) {
    return *((int *)val+1);
}

void ref_inc(void *val) {
    ++(*((int *)val+1));
}

void ref_dec(void *val) {
    --(*((int *)val+1));
}

pyclass *get_class(void *val) {
    return *(pyclass **)((int *)val+2);
}