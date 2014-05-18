#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cpyter.h"
#include "evaluate.h"
#include "execute.h"
#include "gen_evaluate.h"
#include "gen_execute.h"

parenth_form *parenth_form_copy(parenth_form *expr) {
    list *expr_head = list_copy(expr->expr_head);
    return PARENTH_FORM(expr_head);
}

list_expr *list_expr_copy(list_expr *expr) {
    list *expr_head = list_copy(expr->expr_head);
    return LIST_EXPR(expr_head);
}

set_expr *set_expr_copy(set_expr *expr) {
    list *expr_head = list_copy(expr->expr_head);
    return SET_EXPR(expr_head);
}

dict_expr *dict_expr_copy(dict_expr *expr) {
    list *expr_head = list_copy(expr->expr_head);
    list *expr_head2 = list_copy(expr->expr_head2);
    return DICT_EXPR(expr_head, expr_head2);
}

yield_atom *yield_atom_copy(yield_atom *expr) {
    void *expressions = copy(expr->expressions);
    return YIELD_ATOM(expressions);
}

attributeref *attributeref_copy(attributeref *expr) {
    void *primary = copy(expr->primary);
    return ATTRIBUTEREF(primary, expr->id);
}


void *slice_expr_copy(slice_expr *expr) {
    void *start = copy(start);
    void *stop = copy(stop);
    void *step = copy(step);
    return SLICE_EXPR(start, stop, step);
}

void *subsc_expr_copy(subsc_expr *expr) {
    void *value = copy(expr->value);
    return SUBSC_EXPR(value);
}

void *slicing_copy(slicing *expr) {
    void *primary = copy(expr->primary);
    slice_expr *slice = copy(expr->slice);
    return SLICING(primary, slice);
}

void *subscription_copy(subscription *expr) {
    void *primary = copy(expr->primary);
    subsc_expr *subsc = copy(expr->subsc);
    return SUBSCRIPTION(primary, subsc);
}

void *call_copy(call *ptr) {
    return CALL(copy(ptr->primary), list_copy(ptr->arguments));
}

void *power_copy(power *expr) {
    return POWER(copy(expr->primary), copy(expr->u_expr));
}

void *u_expr_copy(u_expr *expr) {
    return U_EXPR(expr->op, copy(expr->expr));
}

void *b_expr_copy(b_expr *expr) {
    return B_EXPR(copy(expr->left), expr->op, copy(expr->right));
}

comparison *comparison_copy(comparison *expr) {
    comparison *retptr = (comparison *)malloc(sizeof(comparison));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = comparison_t;
    retptr->comparisons = list_copy(expr->comparisons);
    return retptr;
}

void *not_test_copy(not_test *expr) {
    return NOT_TEST(copy(expr->expr));
}

void *conditional_expression_copy(conditional_expression *expr) {
    return CONDITIONAL_EXPRESSION(copy(expr->or_test), copy(expr->or_test2), copy(expr->expr));
}

void *expression_list_copy(expression_list *expr) {
    return EXPRESSION_LIST(list_copy(expr->expr_head));
}

/* CAUTION env->val_dict could be an empty list */
void *identifier_gen_evaluate(identifier *structure, environment *env) {
    list *ptr;
    environment *env_ptr;
    if (structure->value) {
        for (env_ptr = env; env_ptr; env_ptr = env_ptr->outer) {
            if (!list_is_empty(env_ptr->val_dict)) {
                for (ptr = env_ptr->val_dict; ptr; ptr = ptr->next) {
                    val_dict_entry *entry = (val_dict_entry *)ptr->content;
                    if (entry->id && !strcmp(entry->id, structure->value)) {
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
                        return entry->value;
                    }
                }
            }
        }
    }
}

void *int_expr_gen_evaluate(int_expr *structure) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = integer__init__(structure->value);
    return retptr;
}

void *float_expr_gen_evaluate(float_expr *structure) {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->value = atof(structure->value);
    return retptr;
}

void *imag_expr_gen_evaluate(imag_expr *structure) {
    pycomplex *retptr = (pycomplex *)malloc(sizeof(pycomplex));
    retptr->type = pycomplex_t;
    char *mag = strdup(structure->value);
    mag[strlen(mag)-1] = 0;
    retptr->real = pyfloat__init__();
    retptr->imag = float_expr_gen_evaluate(FLOAT_EXPR(mag));
    free(mag);
    return retptr;
}

void *str_expr_gen_evaluate(str_expr *structure) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = strdup(strslice(structure->value, 1, -1, 1));
    return retptr;
}

void *parenth_form_gen_evaluate(parenth_form *structure, environment *env) {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    if (list_is_empty(structure->expr_head))
        return retptr;
    if (!structure->ptr)
        structure->ptr = structure->expr_head;
    if (!structure->values)
        structure->values = list_node();
    for ( ; structure->ptr; structure->ptr = structure->ptr->next) {
        void *val = gen_evaluate(structure->ptr->content, env);
        if (env->yield)
            return 0;
        list_append_content(structure->values, val);
    }
    retptr->values = structure->values;
    structure->values = 0;
    return retptr;
}

void *list_expr_gen_evaluate(list_expr *structure, environment *env) {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    if (list_is_empty(structure->expr_head))
        return retptr;
    if (!structure->ptr)
        structure->ptr = structure->expr_head;
    if (!structure->values)
        structure->values = list_node();
    for ( ; structure->ptr; structure->ptr = structure->ptr->next) {
        void *val = gen_evaluate(structure->ptr->content, env);
        if (env->yield)
            return 0;
        list_append_content(structure->values, val);
    }
    retptr->values = structure->values;
    structure->values = 0;
    return retptr;
}

void *list_comprehension_gen_evaluate(list_comprehension *structure, environment *env) {
    environment *local_env = environment_init(env);
    execute(structure->_suite, local_env, 0);
    list *ptr;
    for (ptr = local_env->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (!entry->id)
            return entry->value;
    }
}

void *set_expr_gen_evaluate(set_expr *structure, environment *env) {
    pyset *retptr = (pyset *)malloc(sizeof(pyset));
    retptr->type = pyset_t;
    if (!structure->ptr)
        structure->ptr = structure->expr_head;
    if (!structure->values)
        structure->values = list_node();
    for ( ; structure->ptr; structure->ptr = structure->ptr->next) {
        void *val = gen_evaluate(structure->ptr->content, env);
        if (env->yield)
            return 0;
        if (list_find(retptr->values, val) < 0)
            list_append_content(structure->values, val);
    }
    retptr->values = structure->values;
    structure->values = 0;
    return retptr;
}

void *dict_expr_gen_evaluate(dict_expr *structure, environment *env) {
    pydict *retptr = (pydict *)malloc(sizeof(pydict));
    retptr->type = pydict_t;
    retptr->keys = list_node();
    retptr->values = list_node();
    if (!structure->ptr)
        structure->ptr = structure->expr_head;
    if (!structure->ptr2)
        structure->ptr2 = structure->expr_head2;
    void *key, *val;
    int pos;
    while (structure->ptr) {
        key = gen_evaluate(structure->ptr->content, env);
        if (env->yield)
            return 0;
        val = gen_evaluate(structure->ptr2->content, env);
        if (env->yield)
            return 0;
        if ((pos = list_find(retptr->keys, key)) >= 0)
            list_replace(retptr->values, pos, val);
        else {
            list_append_content(retptr->keys, key);
            list_append_content(retptr->values, val);
        }
        structure->ptr = structure->ptr->next;
        structure->ptr2 = structure->ptr2->next;
    }
    return retptr;
}

void *yield_atom_gen_evaluate(yield_atom *structure, environment *env) {
    if (!structure->yielded) {
        env->yield = gen_evaluate(structure->expressions, env);
        structure->yielded = 1;
        return 0;
    }
    else {
        structure->yielded = 0;
        // if (env->send)
        //     return env->send;
        return pyNone_init();
    }
}

void *attributeref_gen_evaluate(attributeref *structure, environment *env) {
    void *primary_val = gen_evaluate(structure->primary, env);
    if (env->yield)
        return 0;
    list *ptr;
    if (type(primary_val) == instance_t) {
        instance *inst = (instance *)primary_val;
        if (!list_is_empty(inst->env->val_dict)) {
            for (ptr = inst->env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (!strcmp(entry->id, structure->id->value)) {
                    if (type(entry->value) == pyfunction_t)
                        ((pyfunction *)entry->value)->bound = 0;
                    return entry->value;
                }
            }
        }
        return __getattribute__(inst->class, inst, PYSTR(structure->id->value));
    }
    else if (type(primary_val) == pyclass_t) {
        pyclass *class = (pyclass *)primary_val;
        if (!list_is_empty(class->env->val_dict)) {
            for (ptr = class->env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (!strcmp(entry->id, structure->id->value)) {
                    if (type(entry->value) == pyfunction_t)
                        ((pyfunction *)entry->value)->bound = 0;
                    return entry->value;
                }
            }
        }
        return __getattribute__(class->class, class, PYSTR(structure->id->value));
    }
}

void *slice_expr_gen_evaluate(slice_expr *structure, environment *env) {
    pyslice *retptr = (pyslice *)malloc(sizeof(pyslice));
    retptr->type = pyslice_t;
    if (!structure->start_val)
        structure->start_val = gen_evaluate(structure->start, env);
    if (env->yield) {
        structure->start_val = 0;
        return 0;
    }
    if (!structure->stop_val)
        structure->stop_val = gen_evaluate(structure->stop, env);
    if (env->yield) {
        structure->stop_val = 0;
        return 0;
    }
    if (!structure->step_val)
        structure->step_val = gen_evaluate(structure->step, env);
    if (env->yield) {
        structure->step_val = 0;
        return 0;
    }
    retptr->start = structure->start_val;
    retptr->stop = structure->stop_val;
    retptr->step = structure->step_val;
    structure->start_val = 0;
    structure->stop_val = 0;
    structure->step_val = 0;
    return retptr;
}

void *slicing_gen_evaluate(slicing *structure, environment *env) {
    if (!structure->primary_val)
        structure->primary_val = gen_evaluate(structure->primary, env);
    if (env->yield)
        return 0;
    void *primary_val = structure->primary_val;
    void *slice_val = gen_evaluate(structure->slice, env);
    if (env->yield)
        return 0;
    structure->primary_val = 0;
    return __getitem__(primary_val, slice_val);
}

void *subscription_gen_evaluate(subscription *structure, environment *env) {
    if (!structure->primary_val)
        structure->primary_val = gen_evaluate(structure->primary, env);
    if (env->yield)
        return 0;
    void *primary_val = structure->primary_val;
    void *subsc_val = gen_evaluate(structure->subsc->value, env);
    if (env->yield)
        return 0;
    structure->primary_val = 0;
    return __getitem__(primary_val, subsc_val);
}

void *call_gen_evaluate(call *structure, environment *env) {
    if (!structure->primary_val)
        structure->primary_val = gen_evaluate(structure->primary, env);
    if (env->yield)
        return 0;
    void *primary_val = structure->primary_val;
    if (structure->arguments) {
        void *argument_vals = gen_evaluate(structure->arguments, env);
        if (env->yield)
            return 0;
        structure->primary_val = 0;
        if (type(structure->arguments) == parenth_form_t) {
            pytuple *args = pytuple__init__();
            args->values = list_node();
            args->values->content = argument_vals;
            return __call__(primary_val, args);
        }
        return __call__(primary_val, argument_vals);
    }
    else {
        structure->primary_val = 0;
        return __call__(primary_val, 0);
    }
}

void *power_gen_evaluate(power *structure, environment *env) {
    if (!structure->primary_val)
        structure->primary_val = gen_evaluate(structure->primary, env);
    if (env->yield)
        return 0;
    void *primary_val = structure->primary_val;
    void *u_expr_val = gen_evaluate(structure->u_expr, env);
    if (env->yield)
        return 0;
    structure->primary_val = 0;
    if (*(int *)primary_val == pyint_t && *(int *)u_expr_val == pyint_t) {
        integer *val = integer__pow__(((pyint *)primary_val)->value, ((pyint *)u_expr_val)->value);
        return PYINT(val);
    }
}

void *u_expr_gen_evaluate(u_expr *structure, environment *env) {
    void *expr_val = gen_evaluate(structure->expr, env);
    if (env->yield)
        return 0;
    if (*(int *)expr_val == pyint_t) {
        pyint *val = expr_val;
        if (!strcmp(structure->op, "+"))
            return val;
        else if (!strcmp(structure->op, "-")) {
            integer *newvalue = integer__neg__(val->value);
            integer__del__(val->value);
            val->value = newvalue;
            return val;
        }
        else if ((!strcmp(structure->op, "~"))) {
            integer *newvalue = integer__invert__(val->value);
            integer__del__(val->value);
            val->value = newvalue;
            return val;
        }
    }
}

void *b_expr_gen_evaluate(b_expr *structure, environment *env) {
    if (!structure->left_val)
        structure->left_val = gen_evaluate(structure->left, env);
    if (env->yield)
        return 0;
    int *left_val = structure->left_val;
    int *right_val = gen_evaluate(structure->right, env);
    if (env->yield)
        return 0;
    structure->left_val = 0;
    integer *zero = INTEGER_NODE();
    if (!strcmp(structure->op, "*"))
        return __mul__(left_val, right_val);
    else if (!strcmp(structure->op, "/"))
        return __div__(left_val, right_val);
    else if (!strcmp(structure->op, "+"))
        return __add__(left_val, right_val);
    else if (!strcmp(structure->op, "-"))
        return __sub__(left_val, right_val);

    if (*left_val == pyint_t && *right_val == pyint_t) {
        integer *left = ((pyint *)left_val)->value;
        integer *right = ((pyint *)right_val)->value;
        if (!strcmp(structure->op, "//"))
            return pyint__div__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "%"))
            return pyint__mod__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "<<"))
            return pyint__lshift__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, ">>"))
            return pyint__rshift__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "&"))
            return PYINT(integer__and__(left, right));
        else if (!strcmp(structure->op, "^"))
            return PYINT(integer__xor__(left, right));
        else if (!strcmp(structure->op, "|"))
            return PYINT(integer__or__(left, right));
        else if (!strcmp(structure->op, "<"))
            return pyint__lt__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, ">"))
            return PYBOOL(integer__gt__(left, right));
        else if (!strcmp(structure->op, "=="))
            return PYBOOL(integer__eq__(left, right));
        else if (!strcmp(structure->op, "<="))
            return PYBOOL(integer__le__(left, right));
        else if (!strcmp(structure->op, ">="))
            return PYBOOL(integer__ge__(left, right));
        else if (!strcmp(structure->op, "<>"))
            return PYBOOL(!integer__eq__(left, right));
        else if (!strcmp(structure->op, "!="))
            return PYBOOL(!integer__eq__(left, right));
        else if (!strcmp(structure->op, "and")) {
            if (left && right)
            if (!integer__eq__(left, zero) && !integer__eq__(right, zero))
                return PYINT(right);
            return PYINT(zero);
        }
        else if (!strcmp(structure->op, "or")) {
            if (!integer__eq__(left, zero))
                return PYINT(left);
            else if (!integer__eq__(right, zero))
                return PYINT(right);
            return PYINT(0);
        }
    }
}

void *not_test_gen_evaluate(not_test *structure, environment *env) {
    integer *zero = INTEGER_NODE();
    int *expr_val = gen_evaluate(structure->expr, env);
    if (env->yield)
        return 0;
    if (*expr_val == pyint_t) {
        pyint *val = (pyint *)expr_val;
        if (integer__eq__(val->value, zero))
            return PYBOOL(1);
        return PYBOOL(0);
    }
    else if (*expr_val == pystr_t) {
        return PYBOOL(0);
    }
    else if (*expr_val == pybool_t) {
        if (((pybool *)expr_val)->value)
            return PYBOOL(0);
        return PYBOOL(1);
    }
}

void *comparison_gen_evaluate(comparison *structure, environment *env) {
    if (!structure->list_ptr)
        structure->list_ptr = structure->comparisons;
    int *val;
    for ( ; structure->list_ptr; structure->list_ptr = structure->list_ptr->next) {
        val = gen_evaluate(structure->list_ptr->content, env);
        if (env->yield)
            return 0;
        if (type(val) == pyint_t && integer__eq__(((pyint *)val)->value, INTEGER_NODE())) {
            structure->list_ptr = 0;
            return PYBOOL(0);
        }
        else if (type(val) == pybool_t && ((pybool *)val)->value == 0) {
            structure->list_ptr = 0;
            return PYBOOL(0);
        }
    }
    structure->list_ptr = 0;
    return PYBOOL(1);
}

void *conditional_expression_gen_evaluate(conditional_expression *structure, environment *env) {
    integer *zero = INTEGER_NODE();
    if (!structure->retptr)
        structure->retptr = gen_evaluate(structure->or_test, env);
    if (env->yield)
        return 0;
    if (!structure->condptr)
        structure->condptr = gen_evaluate(structure->or_test2, env);
    if (env->yield)
        return 0;
    if (*(int *)structure->condptr == pyint_t && 
              !integer__eq__(((pyint *)structure->condptr)->value, zero))
        return structure->retptr;
    else if (*(int *)structure->condptr == pybool_t && ((pybool *)structure->condptr)->value != 0)
        return structure->retptr;
    else if (*(int *)structure->condptr == pystr_t)
        return structure->retptr;
    if (!structure->retptr2)
        structure->retptr2 = gen_evaluate(structure->expr, env);
    if (env->yield)
        return 0;
    void *retptr = structure->retptr2;
    structure->retptr = 0;
    structure->condptr = 0;
    structure->retptr2 = 0;
    return retptr;
}

void *expression_list_gen_evaluate(expression_list *structure, environment *env) {
    if (!structure->value_list)
        structure->value_list = list_node();
    if (!structure->expr_ptr)
        structure->expr_ptr = structure->expr_head;
    for ( ; structure->expr_ptr; structure->expr_ptr = structure->expr_ptr->next) {
        list_append_content(structure->value_list, gen_evaluate(structure->expr_ptr->content, env));
        if (env->yield)
            return 0;
    }
    pytuple *retptr = pytuple__init__();
    retptr->values = structure->value_list;
    structure->value_list = 0;
    structure->expr_ptr = 0;
    return retptr;
}

void *gen_evaluate(void *structure, environment *env) {
    if (!structure)
        return pyNone_init();
    switch (*(int *)structure) {
        case identifier_t:
            return identifier_gen_evaluate((identifier *)structure, env);
        case int_expr_t:
            return int_expr_gen_evaluate((int_expr *)structure);
        case float_expr_t:
            return float_expr_gen_evaluate((float_expr *)structure);
        case imag_expr_t:
            return imag_expr_gen_evaluate((imag_expr *)structure);
        case str_expr_t:
            return str_expr_gen_evaluate((str_expr *)structure);
        case parenth_form_t:
            return parenth_form_gen_evaluate((parenth_form *)structure, env);
        case list_expr_t:
            return list_expr_gen_evaluate((list_expr *)structure, env);
        case list_comprehension_t:
            return list_comprehension_gen_evaluate((list_comprehension *)structure, env);
        case set_expr_t:
            return set_expr_gen_evaluate((set_expr *)structure, env);
        case dict_expr_t:
            return dict_expr_gen_evaluate((dict_expr *)structure, env);
        case yield_atom_t:
            return yield_atom_gen_evaluate((yield_atom *)structure, env);
        case attributeref_t:
            return attributeref_gen_evaluate((attributeref *)structure, env);
        case slice_expr_t:
            return slice_expr_gen_evaluate((slice_expr *)structure, env);
        case slicing_t:
            return slicing_gen_evaluate((slicing *)structure, env);
        case subscription_t:
            return subscription_gen_evaluate((subscription *)structure, env);
        case call_t:
            return call_gen_evaluate((call *)structure, env);
        case power_t:
            return power_gen_evaluate((power *)structure, env);
        case u_expr_t:
            return u_expr_gen_evaluate((u_expr *)structure, env);
        case b_expr_t:
            return b_expr_gen_evaluate((b_expr *)structure, env);
        case comparison_t:
            return comparison_gen_evaluate((comparison *)structure, env);
        case not_test_t:
            return not_test_gen_evaluate((not_test *)structure, env);
        case conditional_expression_t:
            return conditional_expression_gen_evaluate((conditional_expression *)structure, env);
        case lambda_expr_t:
            return lambda_exprEvaluate((lambda_expr *)structure, env);
        case expression_list_t:
            return expression_list_gen_evaluate((expression_list *)structure, env);
        default:
            printf("[gen_evaluate]: can't gen_evaluate this structure, exit now\n");
            exit(0);
    }
}
