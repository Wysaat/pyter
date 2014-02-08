#define ITEMSIZE 1024
#define STRINGSIZE 1024

enum types { pyId, pyInt, pyStr, pyTuple, 
             pyPower, };

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

int *pyintEvaluate(pyint *structure) {
    int retval = atoi(structure->value);
    return &retval;
}

int *powerEvaluate(power *power) {
}

void *evaluate(void *structure) {
    if (*(int *)structure == pyInt)
        return pyintEvaluate(structure);
}