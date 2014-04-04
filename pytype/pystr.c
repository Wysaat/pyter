#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "pystr.h"
#include "pyint.h"
#include "pybool.h"
#include "../string.h"

pystr *pystr__init__() {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = strdup("");
    retptr->type = pystr_t;
    return retptr;
}

void pystr__del__(pystr *strptr) {
    free(strptr->value);
    free(strptr);
}

pystr *pystr__mul__(void *left, void *right) {
    pystr *strptr = (pystr *)left, *retptr = pystr__init__(), *new_retptr;
    pyint *times = (pyint *)right;
    pyint *zero = pyint__init__();
    zero->value = INTEGER_NODE();
    while (is_true(pyint__gt__(times, zero))) {
        new_retptr = pystr__add__(retptr, strptr);
        pystr__del__(retptr);
        retptr = new_retptr;
        pyint__dec__(times);
    }
    pyint__del__(zero);
    return retptr;
}

pystr *pystr__add__(void *left, void *right) {
    pystr *leftptr = (pystr *)left;
    pystr *rightptr = (pystr *)right;
    pystr *retptr = pystr__init__();
    retptr->value = stradd(leftptr->value, rightptr->value);
    return retptr;
}

pybool *pystr__eq__(void *lvoid, void *rvoid) {
    pystr *left = (pystr *)lvoid;
    pystr *right = (pystr *)rvoid;
    if (!strcmp(left->value, right->value))
        return PYBOOL(1);
    return PYBOOL(0);
}



void pystr__print__(pystr *strptr) {
    printf("%s", strptr->value);
}

void pystr__puts__(pystr *strptr) {
    puts(strptr->value);
}