#include "cpyter.h"

int main() {
    // integer *a = INTEGER_NODE();
    // a->value = 500;
    // integer *b = INTEGER_NODE();
    // b->value = 70432;
    // integer *c = integer__add__(a, b);
    // integer__print__(c);
    int x = 881824;
    char *y = itoa(x);
    printf("y is %s\n", y);
    printf("x is %d\n", x);
    printf("y is %s\n", y);
    return 0;
}