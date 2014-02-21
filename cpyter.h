#include <math.h>

#define ITEMSIZE 1024
#define STRINGSIZE 1024

enum types { pyId, pyInt, pyStr, pyTuple, 
             pyPower, pyU_expr, pyB_expr, pyComparison, 
             pyNot_test, pyConditional_expression,
             pyExpression_list, };

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

typedef struct pyint {
    int type;
    char value[ITEMSIZE];
} pyint;

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

char *last_item();
char *pop_item();
void *parse_u_expr();
void *evaluate(void *);
void *parse_expression();

void *B_EXPR(void *left, char *op, void *right) {
    b_expr *expr = (b_expr *)malloc(sizeof(b_expr));
    expr->type = pyB_expr;
    strcpy(expr->op, op);
    expr->left = left;
    expr->right = right;
    return expr;
}

int *pyintEvaluate(pyint *structure) {
    int retval = atoi(structure->value);
    // return &retval;
    int *retptr = (int *)malloc(sizeof(int));
    *retptr = retval;
    return retptr;
}

int *powerEvaluate(power *structure) {
    int *primary = evaluate(structure->primary);
    int *u_expr = evaluate(structure->u_expr);
    // int *primary = (int *)malloc(sizeof(int));
    // int *u_expr = (int *)malloc(sizeof(int));
    // memcpy(primary, evaluate(structure->primary), sizeof(int));
    // memcpy(u_expr, evaluate(structure->u_expr), sizeof(int));
    int *retptr = (int *)malloc(sizeof(int));
    int retval = pow(*primary, *u_expr);
    *retptr = retval;
    return retptr;
}

int *u_exprEvaluate(u_expr *structure) {
    int *expr = evaluate(structure->expr);
    int retval;
    int *retptr = (int *)malloc(sizeof(int));
    switch (structure->u_op) {
      case '+':
        retval = *expr;
        break;
      case '-':
        retval = -(*expr);
        break;
      case '~':
        retval = ~(*expr);
        break;
    }
    *retptr = retval;
    return retptr;
}

int *b_exprEvaluate(b_expr *structure) {
    int retval;
    int *retptr = (int *)malloc(sizeof(int));
    int *left = evaluate(structure->left);
    int *right = evaluate(structure->right);
    if (match(structure->op, "*"))
        retval = *left * *right;
    else if (match(structure->op, "//"))
        retval = *left / *right;
    else if (match(structure->op, "/"))
        retval = *left / *right;
    else if (match(structure->op, "%"))
        retval = *left % *right;
    else if (match(structure->op, "+"))
        retval = *left + *right;
    else if (match(structure->op, "-"))
        retval = *left - *right;
    else if (match(structure->op, "<<"))
        retval = *left << *right;
    else if (match(structure->op, ">>"))
        retval = *left >> *right;
    else if (match(structure->op, "&"))
        retval = *left & *right;
    else if (match(structure->op, "^"))
        retval = *left ^ *right;
    else if (match(structure->op, "|"))
        retval = *left | *right;
    else if (match(structure->op, "<"))
        retval = *left < *right;
    else if (match(structure->op, ">"))
        retval = *left > *right;
    else if (match(structure->op, "=="))
        retval = *left == *right;
    else if (match(structure->op, "<="))
        retval = *left <= *right;
    else if (match(structure->op, ">="))
        retval = *left >= *right;
    else if (match(structure->op, "<>"))
        retval = *left != *right;
    else if (match(structure->op, "!="))
        retval = *left != *right;
    else if (match(structure->op, "and"))
        retval = *left && *right;
    else if (match(structure->op, "or"))
        retval = *left || *right;
    *retptr = retval;
    return retptr;
}

int *not_testEvaluate(not_test *structure) {
    int *ptr = evaluate(structure->expr);
    int retval = !(*ptr);
    int *retptr = (int *)malloc(sizeof(int));
    *retptr = retval;
    return retptr;
}

void *comparisonEvaluate(comparison *structure) {
    list *ptr;
    int *retptr = (int *)malloc(sizeof(int));
    for (ptr = structure->comparisons; ptr->next != 0; ptr = ptr->next) {
        retptr = evaluate(ptr->next->content);
        if (!(*retptr))
            return retptr;
    }
    return retptr;
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
        case pyInt:
            return pyintEvaluate((pyint *)structure);
        case pyPower:
            return powerEvaluate((power *)structure);
        case pyU_expr:
            return u_exprEvaluate((u_expr *)structure);
        case pyB_expr:
            return b_exprEvaluate((b_expr *)structure);
        case pyComparison:
            return comparisonEvaluate((comparison *)structure);
        case pyNot_test:
            return not_testEvaluate((not_test *)structure);
        case pyConditional_expression:
            return conditional_expressionEvaluate((conditional_expression *)structure);
        case pyExpression_list:
            return expression_listEvaluate((expression_list *)structure);
    }
    // if (*(int *)structure == pyInt)
    //     return pyintEvaluate(structure);
    // else if (*(int *)structure == pyPower)
    //     return powerEvaluate(structure);
    // else if (*(int *)structure == pyU_expr)
    //     return u_exprEvaluate(structure);
}