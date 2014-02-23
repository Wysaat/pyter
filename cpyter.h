#include <math.h>

#define ITEMSIZE 1024
#define STRINGSIZE 1024

enum types { int_expr_t, str_expr_t,
             parenth_form_t,
             power_t, u_expr_t, b_expr_t, comparison_t, 
             not_test_t, conditional_expression_t,
             expression_list_t, 
             pyint_t, pystr_t, pybool_t,
             pytuple_t, };

#define match(x, y) !strcmp(x, y)

/* items ends with " " */
// int in(char *item, char **items) {
//     int i;
//     puts("in function in()");
//     printf("**items, %p\n", *items);
//     for (i = 0; !match(items[i], " "); i++) {
//         if (match(item, items[i]))
//             return 1;
//     }
//     return 0;
// }

typedef struct list list;

typedef struct int_expr {
    int type;
    char value[ITEMSIZE];
} int_expr;

typedef struct str_expr {
    int type;
    char value[ITEMSIZE];
} str_expr;

typedef struct *parenth_form {
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

void list_append(list *head, void *content) {
    list *node = (list *)malloc(sizeof(list));
    node->content = content;
    node->next = 0;
    list *ptr;
    for (ptr = head; ptr->next != 0; ptr = ptr->next)
        ;
    ptr->next = node;
}

void list_add(list *list1, list *list2) {
    list *ptr;
    for (ptr = list1; ptr->next != 0; ptr = ptr->next)
        ;
    ptr->next = list2->next;
    free(list2);
}

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

char *last_item();
char *pop_item();
void *parse_u_expr();
void *evaluate(void *);
void *parse_expression();
pystr *str__mul__(pystr *, pyint *);
pystr *str__add__(pystr *, pystr *);

void *B_EXPR(void *left, char *op, void *right) {
    b_expr *expr = (b_expr *)malloc(sizeof(b_expr));
    expr->type = b_expr_t;
    strcpy(expr->op, op);
    expr->left = left;
    expr->right = right;
    return expr;
}

void *PYINT(int value) {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    retptr->value = value;
    return retptr;
}

void *PYSTR(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    strcpy(retptr->value, value);
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
    retptr->value = atoi(structure->value);
    return retptr;
}

void *str_exprEvaluate(str_expr *structure) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->type = pystr_t;
    strcpy(retptr->value, structure->value+1);
    retptr->value[strlen(retptr->value)-1] = 0;
    return retptr;
}

void *powerEvaluate(power *structure) {
    void *primary_val = evaluate(structure->primary);
    void *u_expr_val = evaluate(structure->u_expr);
    if (*(int *)primary_val == pyint_t && *(int *)u_expr_val == pyint_t) {
        return PYINT(pow(((pyint *)primary_val)->value, ((pyint *)u_expr_val)->value));
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
                val->value = -val->value;
                return val;
            case '~':
                val->value = ~val->value;
                return val;
        }
    }
}

void *b_exprEvaluate(b_expr *structure) {
    int *left_val = evaluate(structure->left);
    int *right_val = evaluate(structure->right);
    if (*left_val == pyint_t && *right_val == pyint_t) {
        int left = ((pyint *)left_val)->value;
        int right = ((pyint *)right_val)->value;
        if (match(structure->op, "*"))
            return PYINT(left * right);
        else if (match(structure->op, "//"))
            return PYINT(left / right);
        else if (match(structure->op, "/"))
            return PYINT(left / right);
        else if (match(structure->op, "%"))
            return PYINT(left % right);
        else if (match(structure->op, "+"))
            return PYINT(left + right);
        else if (match(structure->op, "-"))
            return PYINT(left - right);
        else if (match(structure->op, "<<"))
            return PYINT(left << right);
        else if (match(structure->op, ">>"))
            return PYINT(left >> right);
        else if (match(structure->op, "&"))
            return PYINT(left & right);
        else if (match(structure->op, "^"))
            return PYINT(left ^ right);
        else if (match(structure->op, "|"))
            return PYINT(left | right);
        else if (match(structure->op, "<"))
            return PYBOOL(left < right);
        else if (match(structure->op, ">"))
            return PYBOOL(left > right);
        else if (match(structure->op, "=="))
            return PYBOOL(left == right);
        else if (match(structure->op, "<="))
            return PYBOOL(left <= right);
        else if (match(structure->op, ">="))
            return PYBOOL(left >= right);
        else if (match(structure->op, "<>"))
            return PYBOOL(left != right);
        else if (match(structure->op, "!="))
            return PYBOOL(left != right);
        else if (match(structure->op, "and")) {
            if (left && right)
                return PYINT(right);
            return PYINT(0);
        }
        else if (match(structure->op, "or")) {
            if (left)
                return PYINT(left);
            else if (right)
                return PYINT(right);
            return PYINT(0);
        }
    }
    else if (*left_val == pystr_t && *right_val == pyint_t) {
        if (match(structure->op, "*")) {
            return str__mul__((pystr *)left_val, (pyint *)right_val);
        }
    }
    else if (*left_val == pyint_t && *right_val == pystr_t) {
        if (match(structure->op, "*")) {
            return str__mul__((pystr *)right_val, (pyint *)left_val);
        }
    }
    else if (*left_val == pystr_t && *right_val == pystr_t) {
        if (match(structure->op, "+")) {
            return str__add__((pystr *)left_val, (pystr *)right_val);
        }
    }
}

void *not_testEvaluate(not_test *structure) {
    int *expr_val = evaluate(structure->expr);
    if (*expr_val == pyint_t) {
        pyint *val = (pyint *)expr_val;
        return PYBOOL(!val->value);
    }
    else if (*expr_val == pystr_t) {
        return PYBOOL(0);
    }
}

void *comparisonEvaluate(comparison *structure) {
    list *ptr;
    int *val;
    for (ptr = structure->comparisons; ptr->next != 0; ptr = ptr->next) {
        val = evaluate(ptr->next->content);
        if (*val == pyint_t && ((pyint *)val)->value == 0)
            return PYBOOL(0);
    }
    return PYBOOL(((pyint *)val)->value);
}

void *conditional_expressionEvaluate(conditional_expression *structure) {
    int *retptr = (int *)malloc(sizeof(int));
    int *condptr = (int *)malloc(sizeof(int));
    retptr = evaluate(structure->or_test);
    condptr = evaluate(structure->or_test2);
    if (*condptr)
        return retptr;
    retptr = evaluate(structure->expr);
    return retptr;
}

void *expression_listEvaluate(expression_list *structure) {
    list *expr_ptr;
    list *value_list = (list *)malloc(sizeof(list));
    bzero(value_list, sizeof(list));
    for (expr_ptr = structure->expr_head; expr_ptr->next != 0; expr_ptr = expr_ptr->next) {
        list_append(value_list, evaluate(expr_ptr->next));
    }
    return value_list;
}

void *evaluate(void *structure) {
    switch (*(int *)structure) {
        case int_expr_t:
            return int_exprEvaluate((int_expr *)structure);
        case str_expr_t:
            return str_exprEvaluate((str_expr *)structure);
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
    }
}

void print(void *structure) {
    switch (*(int *)structure) {
        case pyint_t:
            printf("%d\n", ((pyint *)structure)->value);
            break;
        case pystr_t:
            printf("'%s'\n", ((pystr *)structure)->value);
            break;
        case pybool_t:
            if (((pybool *)structure)->value)
                puts("True");
            else
                puts("False");
            break;
    }
}

pystr *str__mul__(pystr *left, pyint *right) {
    int offset = strlen(left->value), i;
    char value[ITEMSIZE];
    char *dest = value;
    for (i = 0; i < right->value; i++) {
        strncpy(dest, left->value, offset);
        dest += offset;
    }
    *dest = 0;
    return PYSTR(value);
}

pystr *str__add__(pystr *left, pystr *right) {
    char value[ITEMSIZE];
    int offset = strlen(left->value);
    strcpy(value, left->value);
    strcpy(value+offset, right->value);
    return PYSTR(value);
}