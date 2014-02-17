#include <math.h>

#define ITEMSIZE 1024
#define STRINGSIZE 1024

enum types { pyId, pyInt, pyStr, pyTuple, 
             pyPower, pyU_expr };

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

char *last_item();
void *parse_u_expr();
void *evaluate(void *);

int *pyintEvaluate(pyint *structure) {
    printf("in pyintEvaluate\n");
    int retval = atoi(structure->value);
    printf("retval is %d\n", retval);
    return &retval;
}

int *powerEvaluate(power *structure) {
    printf("structure->primary->value: %s\n", ((pyint *)structure->primary)->value);
    int *primary = evaluate(structure->primary);
    int *u_expr = evaluate(structure->u_expr);
    int retval = pow(*primary, *u_expr);
    puts("here!!");
    printf("!!!!!: %d %d\n", *primary, *u_expr);
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

void *evaluate(void *structure) {
    switch (*(int *)structure) {
        case pyInt:
            return (void *)pyintEvaluate((pyint *)structure);
        case pyPower:
            puts("power!!!!!");
            return (void *)powerEvaluate((power *)structure);
        case pyU_expr:
            return (void *)u_exprEvaluate((u_expr *)structure);
    }
    // if (*(int *)structure == pyInt)
    //     return pyintEvaluate(structure);
    // else if (*(int *)structure == pyPower)
    //     return powerEvaluate(structure);
    // else if (*(int *)structure == pyU_expr)
    //     return u_exprEvaluate(structure);
}