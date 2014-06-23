#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../types.h"
#include "../struct_info.h"
#include "pyint.h"
#include "../integer.h"
#include "pybool.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pylist.h"
#include "pytuple.h"
#include "../__builtins__.h"

pyint *pyint__init__() {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    memset(retptr, 0, sizeof(pyint));
    retptr->type = pyint_t;
    retptr->class = &int_class;
    return retptr;
}

void pyint__del__(void *vptr) {
    ref_dec(vptr);
    if (get_ref(vptr) == 0) {
        pyint *ptr = (pyint *)vptr;
        integer__del__(ptr->value);
        free(ptr);
    }
}

void pyint_ref(void *vptr) {
    ref_inc(vptr);
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
    else if (type(rvoid) == pystr_t)
        return pystr__mul__(rvoid, lvoid);
    else if (type(rvoid) == pylist_t)
        return pylist__mul__(rvoid, lvoid);
    else if (type(rvoid) == pytuple_t)
        return pytuple__mul__(rvoid, lvoid);
}

void *pyint__div__(void *lvoid, void *rvoid) {
    pyfloat *left = pyint__float__(lvoid);
    void *retptr = pyfloat__div__(left, rvoid);
    pyfloat__del__(left);
    return retptr;
}

void *pyint__rfloordiv__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyint *left = (pyint *)lvoid;
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__div__(left->value, right->value);
        return retptr;
    }
    pyfloat *result = pyint__div__(lvoid, rvoid);
    result->value = floor(result->value);
    return result;
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

pybool *pyint__lt__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    pyint *right = (pyint *)rvoid;
    return PYBOOL(integer__lt__(left->value, right->value));
}

pybool *pyint__gt__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    pyint *right = (pyint *)rvoid;
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
    char sign = '+';
    if (number < 0) {
        number = -number;
        sign = '-';
    }
    retptr->value = integer__init__(itoa(number));
    retptr->value->sign = sign;
    return retptr;
}

int pyint_to_int(pyint *ptr) {
    return (atoi(integer__str__(ptr->value)));
}

pyint *pyint_cpy(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    pyint *retptr = pyint__init__();
    retptr->value = integer__cpy__(ptr->value);
    return retptr;
}

pyint *pyint__neg__(pyint *ptr) {
    pyint *retptr = pyint_cpy(ptr);
    if (retptr->value->sign == '+')
        retptr->value->sign = '-';
    else
        retptr->value->sign = '+';
    return retptr;
}

pyint *pyint__invert__(pyint *ptr) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__invert__(ptr->value);
    return retptr;
}

void *pyint__pow__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyint *left = (pyint *)lvoid;
        pyint *right = (pyint *)rvoid;
        if (right->value->sign == '-') {
            pyfloat *fleft = pyint__float__(lvoid);
            pyfloat *fright = pyint__float__(rvoid);
            void *retptr = pyfloat__pow__(fleft, fright);
            del(fleft);
            del(fright);
            return retptr;
        }
        else {
            pyint *retptr = pyint__init__();
            retptr->value = integer__pow__(left->value, right->value);
            return retptr;
        }
    }
    else if (type(rvoid) == pyfloat_t) {
        pyfloat *fptr = pyint__float__(lvoid);
        void *retptr = pyfloat__pow__(fptr, rvoid);
        del(fptr);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *cptr = pyint__complex__(lvoid);
        void *retptr = pycomplex__pow__(cptr, rvoid);
        del(cptr);
        return retptr;
    }
}

pyint *pyint__int__(void *vptr) {
    return (pyint *)vptr;
}

pyfloat *pyint__float__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    pyfloat *retptr = pyfloat__init__();
    integer *integerp = intptr->value;
    while (integerp) {
        retptr->value += integerp->value * pow(1e1, INTEGER_SZ*integerp->index);
        integerp = integerp->lower;
    }
    if (intptr->value->sign == '-')
        retptr->value = -retptr->value;
    return retptr;
}

pycomplex *pyint__complex__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    pycomplex *retptr = pycomplex__init__();
    integer *integerp = intptr->value;
    while (integerp) {
        retptr->real->value += integerp->value * pow(1e1, INTEGER_SZ*integerp->index);
        integerp = integerp->lower;
    }
    if (intptr->value->sign == '-')
        retptr->real->value = -retptr->real->value;
    return retptr;
}

pystr *pyint__str__(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    char *value = integer__str__(ptr->value);
    pystr *retptr = pystr_init2(value);
    free(value);
    return retptr;
}