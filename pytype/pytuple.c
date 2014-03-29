#include <stdlib.h>
#include "../types.h"
#include "pytuple.h"
#include "pyint.h"

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

pytuple *pytuple__mul__(pytuple *left, void *right) {
    pytuple *retptr = pytuple__init__();
    retptr->values = list_node();
    pyint *times = (pyint *)right;
    pyint *zero = pyint__init__();
    zero->value = INTEGER_NODE();

    while (is_true(pyint__gt__(times, zero))) {
        list *to_append = list_cpy(left->values);
        list_append_list(retptr->values, to_append);
        pyint__dec__(times);
    }
    pyint__del__(zero);

    return retptr;
}