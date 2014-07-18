#include <stdlib.h>
#include "pybool.h"
#include "../types.h"
#include "../struct_info.h"
#include "../builtins/builtins.h"

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
    retptr->class = &bool_class;
    if (value)
        retptr->value = 1;
    else
        retptr->value = 0;
    return retptr;
}

pybool *pybool__bool__(void *ptr) {
    pybool *boolptr = (pybool *)ptr;
    return PYBOOL(boolptr->value);
}

void *pybool__gt__(void *lvoid, void *rvoid) {
    pybool *left = (pybool *)lvoid;
    if (type(rvoid) == pyint_t) {
        return pyint__gt__(int_to_pyint(left->value), rvoid);
    }
    else if (type(rvoid) == pybool_t) {
        return PYBOOL(left->value > ((pybool *)rvoid)->value);
    }
    else if (type(rvoid) == pyfloat_t) {
        return pyint__gt__(int_to_pyint(left->value), rvoid);
    }
}

void *pybool__lt__(void *lvoid, void *rvoid) {
    pybool *left = (pybool *)lvoid;
    if (type(rvoid) == pyint_t) {
        return pyint__lt__(int_to_pyint(left->value), rvoid);
    }
    else if (type(rvoid) == pybool_t) {
        return PYBOOL(left->value < ((pybool *)rvoid)->value);
    }
    else if (type(rvoid) == pyfloat_t) {
        return pyint__lt__(int_to_pyint(left->value), rvoid);
    }
}

void *pybool__eq__(void *lvoid, void *rvoid) {
    pybool *left = (pybool *)lvoid;
    if (type(rvoid) == pyint_t) {
        return pyint__eq__(int_to_pyint(left->value), rvoid);
    }
    else if (type(rvoid) == pybool_t) {
        return PYBOOL(left->value == ((pybool *)rvoid)->value);
    }
    else if (type(rvoid) == pyfloat_t) {
        return pyint__eq__(int_to_pyint(left->value), rvoid);
    }
    return PYBOOL(0);
}

int is_true(pybool *boolval) {
    return boolval->value;
}
