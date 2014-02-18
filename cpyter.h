#include <math.h>

#define ITEMSIZE 1024
#define STRINGSIZE 1024

enum types { pyId, pyInt, pyStr, pyTuple, 
             pyPower, pyU_expr, pyB_expr, };

#define match(x, y) !strcmp(x, y)

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
};

char *last_item();
char *pop_item();
void *parse_u_expr();
void *evaluate(void *);

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
    int retval = pow(*primary, *u_expr);
    return &retval;
}

int *u_exprEvaluate(u_expr *structure) {
    int *expr = evaluate(structure->expr);
    int retval;
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
    return &retval;
}

int *b_exprEvaluate(b_expr *structure) {
    int retval;
    int *retptr = (int *)malloc(sizeof(int));
    int *left = evaluate(structure->left);
    int *right = evaluate(structure->right);
    if (match(structure->op, "*"))
        retval = *left * *right;
    else if (match(structure->op, "/"))
        retval = *left / *right;
    *retptr = retval;
    return retptr;
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
    }
    // if (*(int *)structure == pyInt)
    //     return pyintEvaluate(structure);
    // else if (*(int *)structure == pyPower)
    //     return powerEvaluate(structure);
    // else if (*(int *)structure == pyU_expr)
    //     return u_exprEvaluate(structure);
}