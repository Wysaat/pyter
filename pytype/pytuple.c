#include "../cpyter.h"

pytuple *pytuple__init__() {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    return retptr;
}

pytuple *pytuple__add__(pytuple *left, pytuple *right) {
    pytuple *retptr = pytuple__init__();
    retptr->values = list_add(left->values, right->values);
    return retptr;
}

pytuple *pytuple__mul__(pytuple *left, pyint *right) {
    pytuple *retptr = pytuple__init__();
}