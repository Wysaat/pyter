#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cpyter.h"
#include "evaluate.h"

#include "pytype/methods.h"
#include "pytype/others.h"

void *IDENTIFIER(char *token) {
    identifier *retptr = (identifier *)malloc(sizeof(identifier));
    retptr->type = identifier_t;
    retptr->value = strdup(token);
    return retptr;
}

/* op should be copied(use memory.c function mem_cpy) */
void *INT_EXPR(char *token) {
    int_expr *retptr = (int_expr *)malloc(sizeof(int_expr));
    retptr->type = int_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *FLOAT_EXPR(char *token) {
    float_expr *retptr = (float_expr *)malloc(sizeof(float_expr));
    retptr->type = float_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *IMAG_EXPR(char *token) {
    imag_expr *retptr = (imag_expr *)malloc(sizeof(imag_expr));
    retptr->type = imag_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *STR_EXPR(char *token) {
    str_expr *retptr = (str_expr *)malloc(sizeof(str_expr));
    retptr->type = str_expr_t;
    retptr->value = strdup(token);
    return retptr;
}

void *PARENTH_FORM(list *expr_head) {
    parenth_form *retptr = (parenth_form *)malloc(sizeof(parenth_form));
    retptr->type = parenth_form_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *LIST_EXPR(list *expr_head) {
    list_expr *retptr = (list_expr *)malloc(sizeof(list_expr));
    retptr->type = list_expr_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *SET_EXPR(list *expr_head) {
    set_expr *retptr = (set_expr *)malloc(sizeof(set_expr));
    retptr->type = set_expr_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *DICT_EXPR(list *expr_head, list *expr_head2) {
    dict_expr *retptr = (dict_expr *)malloc(sizeof(dict_expr));
    retptr->type = dict_expr_t;
    retptr->expr_head = expr_head;
    retptr->expr_head2 = expr_head2;
    return retptr;
}

void *SLICE_EXPR(void *start, void *stop, void *step) {
    slice_expr *retptr = (slice_expr *)malloc(sizeof(slice_expr));
    retptr->type = slice_expr_t;
    retptr->start = start;
    retptr->stop = stop;
    retptr->step = step;
    return retptr;
}

void *SUBSC_EXPR(void *value) {
    subsc_expr *retptr = (subsc_expr *)malloc(sizeof(subsc_expr));
    retptr->type = subsc_expr_t;
    retptr->value = value;
    return retptr;
}

void *SLICING(void *primary, slice_expr *slice) {
    slicing *retptr = (slicing *)malloc(sizeof(slicing));
    retptr->type = slicing_t;
    retptr->primary = primary;
    retptr->slice = slice;
    return retptr;
}

void *SUBSCRIPTION(void *primary, subsc_expr *subsc) {
    subscription *retptr = (subscription *)malloc(sizeof(subscription));
    retptr->type = subscription_t;
    retptr->primary = primary;
    retptr->subsc = subsc;
    return retptr;
}

void *CALL(void *primary, void *arguments) {
    call *retptr = (call *)malloc(sizeof(call));
    retptr->type = call_t;
    retptr->primary = primary;
    retptr->arguments = arguments;
    return retptr;
}

void *POWER(void *primary, void *u_expr) {
    power *retptr = (power *)malloc(sizeof(power));
    retptr->type = power_t;
    retptr->primary = primary;
    retptr->u_expr = u_expr;
    return retptr;
}

void *U_EXPR(char *op, void *expr) {
    u_expr *retptr = (u_expr *)malloc(sizeof(u_expr));
    retptr->type = u_expr_t;
    retptr->op = strdup(op);
    retptr->expr = expr;
    return retptr;
}

void *B_EXPR(void *left, char *op, void *right) {
    b_expr *expr = (b_expr *)malloc(sizeof(b_expr));
    expr->type = b_expr_t;
    expr->op = strdup(op);
    expr->left = left;
    expr->right = right;
    return expr;
}

void *NOT_TEST(void *expr) {
    not_test *test = (not_test *)malloc(sizeof(not_test));
    test->type = not_test_t;
    test->expr = expr;
    return test;
}

void *CONDITIONAL_EXPRESSION(void *or_test, void *or_test2, void *expr) {
    conditional_expression *expression = \
        (conditional_expression *)malloc(sizeof(conditional_expression));
    expression->type = conditional_expression_t;
    expression->or_test = or_test;
    expression->or_test2 = or_test2;
    expression->expr = expr;
    return expression;
}

void *EXPRESSION_LIST(list *expr_head) {
    expression_list *retptr = (expression_list *)malloc(sizeof(expression_list));
    retptr->type = expression_list_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *PYINT(integer *value) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = value;
    return retptr;
}

void *PYSTR(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = strdup(value);
    return retptr;
}

void *identifierEvaluate(identifier *structure, environment *global) {
    list *ptr;
    for (ptr = global->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (!strcmp(entry->id, structure->value))
            return entry->value;
    }
}

void *int_exprEvaluate(int_expr *structure) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = integer__init__(structure->value);
    return retptr;
}

void *float_exprEvaluate(float_expr *structure) {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->value = atof(structure->value);
    return retptr;
}

void *imag_exprEvaluate(imag_expr *structure) {
    pycomplex *retptr = (pycomplex *)malloc(sizeof(pycomplex));
    retptr->type = pycomplex_t;
    char *mag = strdup(structure->value);
    mag[strlen(mag)-1] = 0;
    retptr->real = pyfloat__init__();
    retptr->imag = float_exprEvaluate(FLOAT_EXPR(mag));
    free(mag);
    return retptr;
}

void *str_exprEvaluate(str_expr *structure) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = strdup(strslice(structure->value, 1, -1, 1));
    return retptr;
}

void *parenth_formEvaluate(parenth_form *structure, environment *env) {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        list_append_content(retptr->values, evaluate(ptr->content, env));
    }
    return retptr;
}

void *list_exprEvaluate(list_expr *structure, environment *env) {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next)
        list_append_content(retptr->values, evaluate(ptr->content, env));
    return retptr;
}

void *set_exprEvaluate(set_expr *structure, environment *env) {
    pyset *retptr = (pyset *)malloc(sizeof(pyset));
    retptr->type = pyset_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        void *val = evaluate(ptr->content, env);
        if (list_find(retptr->values, val) < 0)
            list_append_content(retptr->values, val);
    }
    return retptr;
}

void *dict_exprEvaluate(dict_expr *structure, environment *env) {
    pydict *retptr = (pydict *)malloc(sizeof(pydict));
    retptr->type = pydict_t;
    retptr->keys = list_node();
    retptr->values = list_node();
    list *ptr = structure->expr_head, *ptr2 = structure->expr_head2;
    void *key, *val;
    int pos;
    while (ptr) {
        key = evaluate(ptr->content, env);
        val = evaluate(ptr2->content, env);
        if ((pos = list_find(retptr->keys, key)) >= 0)
            list_replace(retptr->values, pos, val);
        else {
            list_append_content(retptr->keys, key);
            list_append_content(retptr->values, val);
        }
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }
    return retptr;
}

void *slice_exprEvaluate(slice_expr *structure, environment *env) {
    pyslice *retptr = (pyslice *)malloc(sizeof(pyslice));
    retptr->type = pyslice_t;
    retptr->start = evaluate(structure->start, env);
    retptr->stop = evaluate(structure->stop, env);
    retptr->step = evaluate(structure->step, env);
    return retptr;
}

void *slicingEvaluate(slicing *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *slice_val = evaluate(structure->slice, env);
    return __getitem__(primary_val, slice_val);
}

void *subscriptionEvaluate(subscription *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *subsc_val = evaluate(structure->subsc->value, env);
    return __getitem__(primary_val, subsc_val);
}

void *callEvaluate(call *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *argument_vals = evaluate(structure->arguments, env);
    return __call__(primary_val, argument_vals);
}

void *powerEvaluate(power *structure, environment *env) {
    void *primary_val = evaluate(structure->primary, env);
    void *u_expr_val = evaluate(structure->u_expr, env);
    if (*(int *)primary_val == pyint_t && *(int *)u_expr_val == pyint_t) {
        integer *val = integer__pow__(((pyint *)primary_val)->value, ((pyint *)u_expr_val)->value);
        return PYINT(val);
    }
}

void *u_exprEvaluate(u_expr *structure, environment *env) {
    void *expr_val = evaluate(structure->expr, env);
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

void *b_exprEvaluate(b_expr *structure, environment *env) {
    int *left_val = evaluate(structure->left, env);
    int *right_val = evaluate(structure->right, env);
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

void *not_testEvaluate(not_test *structure, environment *env) {
    integer *zero = INTEGER_NODE();
    int *expr_val = evaluate(structure->expr, env);
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

void *comparisonEvaluate(comparison *structure, environment *env) {
    list *ptr;
    int *val;
    for (ptr = structure->comparisons; ptr != 0; ptr = ptr->next) {
        val = evaluate(ptr->content, env);
        if (type(val) == pyint_t && integer__eq__(((pyint *)val)->value, INTEGER_NODE()))
            return PYBOOL(0);
        else if (type(val) == pybool_t && ((pybool *)val)->value == 0)
            return PYBOOL(0);
    }
    return PYBOOL(1);
}

void *conditional_expressionEvaluate(conditional_expression *structure, environment *env) {
    void *retptr, *condptr;
    integer *zero = INTEGER_NODE();
    retptr = evaluate(structure->or_test, env);
    condptr = evaluate(structure->or_test2, env);
    if (*(int *)condptr == pyint_t && !integer__eq__(((pyint *)condptr)->value, zero))
        return retptr;
    else if (*(int *)condptr == pybool_t && ((pybool *)condptr)->value != 0)
        return retptr;
    else if (*(int *)condptr == pystr_t)
        return retptr;
    retptr = evaluate(structure->expr, env);
    return retptr;
}

void *expression_listEvaluate(expression_list *structure, environment *env) {
    list *expr_ptr;
    list *value_list = list_node();
    for (expr_ptr = structure->expr_head; expr_ptr; expr_ptr = expr_ptr->next) {
        list_append_content(value_list, evaluate(expr_ptr->content, env));
    }
    pytuple *retptr = pytuple__init__();
    retptr->values = value_list;
    return retptr;
}

void *evaluate(void *structure, environment *env) {
    switch (*(int *)structure) {
        case identifier_t:
            return identifierEvaluate((identifier *)structure, env);
        case int_expr_t:
            return int_exprEvaluate((int_expr *)structure);
        case float_expr_t:
            return float_exprEvaluate((float_expr *)structure);
        case imag_expr_t:
            return imag_exprEvaluate((imag_expr *)structure);
        case str_expr_t:
            return str_exprEvaluate((str_expr *)structure);
        case parenth_form_t:
            return parenth_formEvaluate((parenth_form *)structure, env);
        case list_expr_t:
            return list_exprEvaluate((list_expr *)structure, env);
        case set_expr_t:
            return set_exprEvaluate((set_expr *)structure, env);
        case dict_expr_t:
            return dict_exprEvaluate((dict_expr *)structure, env);
        case slice_expr_t:
            return slice_exprEvaluate((slice_expr *)structure, env);
        case slicing_t:
            return slicingEvaluate((slicing *)structure, env);
        case subscription_t:
            return subscriptionEvaluate((subscription *)structure, env);
        case call_t:
            return callEvaluate((call *)structure, env);
        case power_t:
            return powerEvaluate((power *)structure, env);
        case u_expr_t:
            return u_exprEvaluate((u_expr *)structure, env);
        case b_expr_t:
            return b_exprEvaluate((b_expr *)structure, env);
        case comparison_t:
            return comparisonEvaluate((comparison *)structure, env);
        case not_test_t:
            return not_testEvaluate((not_test *)structure, env);
        case conditional_expression_t:
            return conditional_expressionEvaluate((conditional_expression *)structure, env);
        case expression_list_t:
            return expression_listEvaluate((expression_list *)structure, env);
        default:
            printf("[evaluate]: can't evaluate this structure, exit now\n");
            exit(0);
    }
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
            printf("'");
            printf("%s", ((pystr *)structure)->value);
            printf("'");
            break;
        case pybool_t:
            if (((pybool *)structure)->value)
                printf("True");
            else
                printf("False");
            break;
        case pytuple_t:
            printf("(");
            for (ptr = ((pytuple *)structure)->values; ptr; ptr = ptr->next) {
                print_nnl(ptr->content);
                if (ptr->next)
                    printf(", ");
            }
            printf(")");
            break;
        case pylist_t:
            printf("[");
            for (ptr = ((pylist *)structure)->values; ptr; ptr = ptr->next) {
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
    }
}

/* '\n' append */
void print(void *structure) {
    if (!structure)
         return;
    if (type(structure) == list_t) {
        list *vallist = (list *)structure, *ptr;
        for (ptr = vallist; ptr; ptr = ptr->next)
            print(ptr->content);
    }
    else {
        print_nnl(structure);
        printf("\n");
    }
}

int type(void *val) {
    return *(int *)val;
}