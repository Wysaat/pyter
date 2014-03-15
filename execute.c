#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cpyter.h"

/* op should be copied(use memory.c function mem_cpy) */
void *B_EXPR(void *left, mem_block *op, void *right) {
    b_expr *expr = (b_expr *)malloc(sizeof(b_expr));
    expr->type = b_expr_t;
    expr->op = mem_head();
    mem_cpy(expr->op, op);
    expr->left = left;
    expr->right = right;
    return expr;
}

void *PYINT(integer *value) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = value;
    return retptr;
}

void *PYSTR(mem_block *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = value;
    return retptr;
}

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
    retptr->value = value;
    return retptr;
}

void *PARENTH_FORM(list *expr_head) {
    parenth_form *retptr = (parenth_form *)malloc(sizeof(parenth_form));
    retptr->type = parenth_form_t;
    retptr->expr_head = expr_head;
    return retptr;
}

void *int_exprEvaluate(int_expr *structure) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = integer__init__(structure->value);
    return retptr;
}

void *str_exprEvaluate(str_expr *structure) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    retptr->value = mem_head();
    mem_ncpy(retptr->value, structure->value, 0, 1, mem_size(structure->value)-2);
    // mem_cpy(retptr->value, structure->value+1);
    // puts("in str_exprEvaluate, after mem_cpy");
    // mem_set(retptr->value, mem_size(retptr->value)-1, 0);
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
        switch(structure->u_op) {
            case '+':
                return val;
            case '-':
                val->value = integer__neg__(val->value);
                return val;
            case '~':
                val->value = integer__invert__(val->value);
                return val;
        }
    }
}

void *b_exprEvaluate(b_expr *structure) {
    // puts("in b_exprEvaluate");
    int *left_val = evaluate(structure->left);
    int *right_val = evaluate(structure->right);
    integer *zero = INTEGER_NODE();
    if (*left_val == pyint_t && *right_val == pyint_t) {
        integer *left = ((pyint *)left_val)->value;
        // printf("left is %d\n", left);
        integer *right = ((pyint *)right_val)->value;
        // printf("right is %d\n", right);
        // puts("print op in the next line");
        // mem_print(structure->op);
        if (mem_match_str(structure->op, "*"))
            return pyint__mul__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "//"))
            return pyint__div__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "/"))
            return pyint__div__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "%"))
            return pyint__mod__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "+"))
            return pyint__add__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "-"))
            return pyint__sub__((pyint *)left_val, (pyint *)right_val);
        else if (mem_match_str(structure->op, "<<"))
            return PYINT(integer__lshift__(left, right));
        else if (mem_match_str(structure->op, ">>"))
            return PYINT(integer__rshift__(left, right));
        else if (mem_match_str(structure->op, "&"))
            return PYINT(integer__and__(left, right));
        else if (mem_match_str(structure->op, "^"))
            return PYINT(integer__xor__(left, right));
        else if (mem_match_str(structure->op, "|"))
            return PYINT(integer__or__(left, right));
        else if (mem_match_str(structure->op, "<"))
            return PYBOOL(integer__lt__(left, right));
        else if (mem_match_str(structure->op, ">"))
            return PYBOOL(integer__gt__(left, right));
        else if (mem_match_str(structure->op, "=="))
            return PYBOOL(integer__eq__(left, right));
        else if (mem_match_str(structure->op, "<="))
            return PYBOOL(integer__le__(left, right));
        else if (mem_match_str(structure->op, ">="))
            return PYBOOL(integer__ge__(left, right));
        else if (mem_match_str(structure->op, "<>"))
            return PYBOOL(!integer__eq__(left, right));
        else if (mem_match_str(structure->op, "!="))
            return PYBOOL(!integer__eq__(left, right));
        else if (mem_match_str(structure->op, "and")) {
            if (left && right)
            if (!integer__eq__(left, zero) && !integer__eq__(right, zero))
                return PYINT(right);
            return PYINT(zero);
        }
        else if (mem_match_str(structure->op, "or")) {
            if (!integer__eq__(left, zero))
                return PYINT(left);
            else if (!integer__eq__(right, zero))
                return PYINT(right);
            return PYINT(0);
        }
    }
    else if (*left_val == pystr_t && *right_val == pyint_t) {
        if (mem_match_str(structure->op, "*")) {
            return pystr__mul__((pystr *)left_val, (pyint *)right_val);
        }
    }
    else if (*left_val == pyint_t && *right_val == pystr_t) {
        if (mem_match_str(structure->op, "*"))
            return pystr__mul__((pystr *)right_val, (pyint *)left_val);
    }
    else if (*left_val == pystr_t && *right_val == pystr_t) {
        if (mem_match_str(structure->op, "+"))
            return pystr__add__((pystr *)left_val, (pystr *)right_val);
    }
    else if (*left_val == pytuple_t && *right_val == pytuple_t) {
        if (mem_match_str(structure->op, "+"))
            return pytuple__add__((pytuple *)left_val, (pytuple *)right_val);
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
    switch (*(int *)structure) {
        case pyint_t:
            mem_print_nnl(integer__str__(((pyint *)structure)->value));
            break;
        case pystr_t:
            mem_print_nnl(((pystr *)structure)->value);
            break;
        case pybool_t:
            if (((pybool *)structure)->value)
                printf("True");
            else
                printf("False");
            break;
        case pytuple_t:
            printf("(");
            list *ptr;
            for (ptr = ((pytuple *)structure)->values; ptr; ptr = ptr->next) {
                print_nnl(ptr->content);
                if (ptr->next)
                    printf(", ");
            }
            printf(")");
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

pystr *pystr__mul__(pystr *left, pyint *right) {
    mem_block *dest = mem_head();
    mem_block *src = left->value;
    integer *times = right->value, *zero = INTEGER_NODE();
    int off = mem_size(left->value), offset;
    for (offset = 0; integer__gt__(times, zero); times = integer__dec__(times), offset += off) {
        mem_ncpy(dest, src, offset, 0, off);
    }
    return PYSTR(dest);
}

pystr *pystr__add__(pystr *left, pystr *right) {
    mem_block *dest = mem_head();
    mem_cpy(dest, left->value);
    mem_ncpy(dest, right->value, mem_size(dest), 0, mem_size(right->value));
    return PYSTR(dest);
}