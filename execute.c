#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cpyter.h"
#include "execute.h"

#include "pytype/methods.h"
#include "pytype/others.h"

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

void *int_exprEvaluate(int_expr *structure) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = integer__init__(structure->value);
    return retptr;
}

void *float_exprEvaluate(float_expr *structure) {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->value = float_init(structure->value);
    return retptr;
}

void *str_exprEvaluate(str_expr *structure) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = strdup(strslice(structure->value, 1, -1, 1));
    return retptr;
}

void *parenth_formEvaluate(parenth_form *structure) {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        list_append_content(retptr->values, evaluate(ptr->content));
    }
    return retptr;
}

void *list_exprEvaluate(list_expr *structure) {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next)
        list_append_content(retptr->values, evaluate(ptr->content));
    return retptr;
}

void *set_exprEvaluate(set_expr *structure) {
    pyset *retptr = (pyset *)malloc(sizeof(pyset));
    retptr->type = pyset_t;
    retptr->values = list_node();
    list *ptr;
    for (ptr = structure->expr_head; ptr; ptr = ptr->next) {
        void *val = evaluate(ptr->content);
        if (list_find(retptr->values, val) < 0)
            list_append_content(retptr->values, val);
    }
    return retptr;
}

void *dict_exprEvaluate(dict_expr *structure) {
    pydict *retptr = (pydict *)malloc(sizeof(pydict));
    retptr->type = pydict_t;
    retptr->keys = list_node();
    retptr->values = list_node();
    list *ptr = structure->expr_head, *ptr2 = structure->expr_head2;
    void *key, *val;
    int pos;
    while (ptr) {
        key = evaluate(ptr->content);
        val = evaluate(ptr2->content);
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

void *slice_exprEvaluate(slice_expr *structure) {
    pyslice *retptr = (pyslice *)malloc(sizeof(pyslice));
    retptr->type = pyslice_t;
    retptr->start = evaluate(structure->start);
    retptr->stop = evaluate(structure->stop);
    retptr->step = evaluate(structure->step);
    return retptr;
}

void *slicingEvaluate(slicing *structure) {
    void *primary_val = evaluate(structure->primary);
    void *slice_val = evaluate(structure->slice);
    return __getitem__(primary_val, slice_val);
}

void *subscriptionEvaluate(subscription *structure) {
    void *primary_val = evaluate(structure->primary);
    void *subsc_val = evaluate(structure->subsc->value);
    return __getitem__(primary_val, subsc_val);
}

void *powerEvaluate(power *structure) {
    void *primary_val = evaluate(structure->primary);
    void *u_expr_val = evaluate(structure->u_expr);
    if (*(int *)primary_val == pyint_t && *(int *)u_expr_val == pyint_t) {
        integer *val = integer__pow__(((pyint *)primary_val)->value, ((pyint *)u_expr_val)->value);
        return PYINT(val);
    }
}

void *u_exprEvaluate(u_expr *structure) {
    void *expr_val = evaluate(structure->expr);
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

void *b_exprEvaluate(b_expr *structure) {
    int *left_val = evaluate(structure->left);
    int *right_val = evaluate(structure->right);
    integer *zero = INTEGER_NODE();
    if (*left_val == pyint_t && *right_val == pyint_t) {
        integer *left = ((pyint *)left_val)->value;
        integer *right = ((pyint *)right_val)->value;
        if (!strcmp(structure->op, "*"))
            return pyint__mul__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "//"))
            return pyint__div__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "/"))
            return pyint__div__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "%"))
            return pyint__mod__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "+"))
            return pyint__add__((pyint *)left_val, (pyint *)right_val);
        else if (!strcmp(structure->op, "-"))
            return pyint__sub__((pyint *)left_val, (pyint *)right_val);
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
    else if (*left_val == pystr_t && *right_val == pyint_t) {
        if (!strcmp(structure->op, "*")) {
            return pystr__mul__((pystr *)left_val, (pyint *)right_val);
        }
    }
    else if (*left_val == pyint_t && *right_val == pystr_t) {
        if (!strcmp(structure->op, "*"))
            return pystr__mul__((pystr *)right_val, (pyint *)left_val);
    }
    else if (*left_val == pystr_t && *right_val == pystr_t) {
        if (!strcmp(structure->op, "+"))
            return pystr__add__((pystr *)left_val, (pystr *)right_val);
    }
    else if (*left_val == pytuple_t) {
        if (!strcmp(structure->op, "+"))
            return pytuple__add__((pytuple *)left_val, (pytuple *)right_val);
        else if(!strcmp(structure->op, "*"))
            return pytuple__mul__((pytuple *)left_val, right_val);
    }
    else if (*left_val == pylist_t) {
        if (!strcmp(structure->op, "+"))
            return pylist__add__(left_val, right_val);
        else if (!strcmp(structure->op, "*"))
            return pylist__mul__(left_val, right_val);
    }
}

void *not_testEvaluate(not_test *structure) {
    integer *zero = INTEGER_NODE();
    int *expr_val = evaluate(structure->expr);
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

void *comparisonEvaluate(comparison *structure) {
    list *ptr;
    int *val;
    for (ptr = structure->comparisons; ptr != 0; ptr = ptr->next) {
        val = evaluate(ptr->content);
        if (*val == pyint_t && integer__eq__(((pyint *)val)->value, INTEGER_NODE()))
            return PYBOOL(0);
        else if (*val == pybool_t && ((pybool *)val)->value == 0)
            return PYBOOL(0);
    }
    return PYBOOL(1);
}

void *conditional_expressionEvaluate(conditional_expression *structure) {
    // int *retptr = (int *)malloc(sizeof(int));
    // int *condptr = (int *)malloc(sizeof(int));
    // retptr = evaluate(structure->or_test);
    // condptr = evaluate(structure->or_test2);
    // if (*condptr)
    //     return retptr;
    // retptr = evaluate(structure->expr);
    // return retptr;

    void *retptr, *condptr;
    integer *zero = INTEGER_NODE();
    retptr = evaluate(structure->or_test);
    condptr = evaluate(structure->or_test2);
    if (*(int *)condptr == pyint_t && !integer__eq__(((pyint *)condptr)->value, zero))
        return retptr;
    else if (*(int *)condptr == pybool_t && ((pybool *)condptr)->value != 0)
        return retptr;
    else if (*(int *)condptr == pystr_t)
        return retptr;
    retptr = evaluate(structure->expr);
    return retptr;
}

void *expression_listEvaluate(expression_list *structure) {
    list *expr_ptr;
    list *value_list = list_node();
    for (expr_ptr = structure->expr_head; expr_ptr != 0; expr_ptr = expr_ptr->next) {
        list_append_content(value_list, evaluate(expr_ptr->next));
    }
    return value_list;
}

void *evaluate(void *structure) {
    switch (*(int *)structure) {
        case int_expr_t:
            return int_exprEvaluate((int_expr *)structure);
        case str_expr_t:
            return str_exprEvaluate((str_expr *)structure);
        case parenth_form_t:
            return parenth_formEvaluate((parenth_form *)structure);
        case list_expr_t:
            return list_exprEvaluate((list_expr *)structure);
        case set_expr_t:
            return set_exprEvaluate((set_expr *)structure);
        case dict_expr_t:
            return dict_exprEvaluate((dict_expr *)structure);
        case slice_expr_t:
            return slice_exprEvaluate((slice_expr *)structure);
        case slicing_t:
            return slicingEvaluate((slicing *)structure);
        case subscription_t:
            return subscriptionEvaluate((subscription *)structure);
        case power_t:
            return powerEvaluate((power *)structure);
        case u_expr_t:
            return u_exprEvaluate((u_expr *)structure);
        case b_expr_t:
            return b_exprEvaluate((b_expr *)structure);
        case comparison_t:
            return comparisonEvaluate((comparison *)structure);
        case not_test_t:
            return not_testEvaluate((not_test *)structure);
        case conditional_expression_t:
            return conditional_expressionEvaluate((conditional_expression *)structure);
        case expression_list_t:
            return expression_listEvaluate((expression_list *)structure);
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
        default:
            printf("[print_nnl]: unsupported structure\n");
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