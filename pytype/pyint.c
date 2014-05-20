#include <stdio.h>
#include <stdlib.h>
#include "../types.h"
#include "pyint.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pybool.h"
#include "../evaluate.h"
#include <math.h>

pyint *pyint__init__() {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    return retptr;
}

void pyint__del__(pyint *val) {
    integer__del__(val->value);
    free(val);
}

pyfloat *pyint__float__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    pyfloat *retptr = pyfloat__init__();
    integer *integerp = intptr->value;
    while (integerp) {
        retptr->value += integerp->value * pow(1e1, INTEGER_SZ*integerp->index);
        integerp = integerp->lower;
    }
    return retptr;
}

pybool *pyint__bool__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    integer *zero = INTEGER_NODE();
    pybool *retptr;
    if (integer__eq__(intptr->value, zero))
        retptr = PYBOOL(0);
    else
        retptr = PYBOOL(1);
    free(zero);
    return retptr;
}

void *pyint__add__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__add__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pycomplex_t) {
        pyfloat *fleft = pyint__float__(left);
        void *retptr = pyfloat__add__(fleft, rvoid);
        pyfloat__del__(fleft);
        return retptr;
    }
}

void *pyint__sub__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__sub__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pycomplex_t) {
        pyfloat *fleft = pyint__float__(left);
        void *retptr = pyfloat__sub__(fleft, rvoid);
        pyfloat__del__(fleft);
        return retptr;
    }
}

void *pyint__mul__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__mul__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t)
        return pyfloat__mul__(rvoid, lvoid);
    else if (type(rvoid) == pycomplex_t)
        return pycomplex__mul__(rvoid, lvoid);
    else if (type(rvoid) == pystr_t) {
        return pystr__mul__(rvoid, lvoid);
    }
}

void *pyint__div__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__div__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pycomplex_t) {
        pyfloat *fleft = pyint__float__(lvoid);
        void *retptr = pyfloat__div__(fleft, rvoid);
        pyfloat__del__(fleft);
        return retptr;
    }
}

pyint *pyint__mod__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__mod__(left->value, right->value);
    return retptr;
}

pyint *pyint__lshift__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__lshift__(left->value, right->value);
    return retptr;
}

pyint *pyint__rshift__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__rshift__(left->value, right->value);
    return retptr;
}

pybool *pyint__lt__(pyint *left, pyint *right) {
    return PYBOOL(integer__lt__(left->value, right->value));
}

pybool *pyint__gt__(pyint *left, pyint *right) {
    return PYBOOL(integer__gt__(left->value, right->value));
}

int pyint__cmp__(pyint *left, pyint *right) {
    return integer__cmp__(left->value, right->value);
}


void pyint__print__(pyint *val) {
    char *string = integer__str__(val->value);
    printf("%s", string);
    free(string);
}

void pyint__puts__(pyint *val) {
    char *string = integer__str__(val->value);
    puts(string);
    free(string);
}

void pyint__dec__(pyint *val) {
    integer *new_value = integer__dec__(val->value);
    integer__del__(val->value);
    val->value = new_value;
}

void pyint__inc__(pyint *val) {
    integer *new_value = integer__inc__(val->value);
    integer__del__(val->value);
    val->value = new_value;
}

pybool *pyint__eq__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    pyint *right = (pyint *)rvoid;
    if (integer__eq__(left->value, right->value))
        return PYBOOL(1);
    return PYBOOL(0);
}

pybool *pyint__ge__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    pyint *right = (pyint *)rvoid;
    if (integer__ge__(left->value, right->value))
        return PYBOOL(1);
    return PYBOOL(0);
}

pyint *int_to_pyint(int number) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__init__(itoa(number));
    return retptr;
}