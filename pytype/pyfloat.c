#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pyfloat.h"
#include "../types.h"
#include "../struct_info.h"
#include "pyint.h"
#include "pycomplex.h"
#include "../builtins/builtins.h"

pyfloat *pyfloat__init__() {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->class = &float_class;
    retptr->value = (double)0.0;
    return retptr;
}

pyfloat *pyfloat_init2(double value) {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->class = &float_class;
    retptr->value = value;
    return retptr;
}

void *pyfloat__mul__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        pyfloat *right = (pyfloat *)rvoid;
        pyfloat *retptr = pyfloat__init__();
        retptr->value = left->value * right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *fright = pyint__float__(rvoid);
        pyfloat *retptr = pyfloat__mul__(lvoid, fright);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t)
        return pycomplex__mul__(rvoid, lvoid);
}

void *pyfloat__div__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        pyfloat *right = (pyfloat *)rvoid;
        pyfloat *retptr = pyfloat__init__();
        retptr->value = left->value / right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *fright = pyint__float__(rvoid);
        pyfloat *retptr = pyfloat__div__(lvoid, fright);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *x = pycomplex__init__();
        x->real->value = right->real->value;
        x->imag->value = -(right->imag->value);
        pycomplex *retptr = pyfloat__mul__(left, x);
        pycomplex *y = pycomplex__mul__(right, x);
        retptr->real->value /= y->real->value;
        retptr->imag->value /= y->real->value;
        return retptr;
    }
}

void *pyfloat__rfloordiv__(void *lvoid, void *rvoid) {
    pyfloat *result = pyfloat__div__(lvoid, rvoid);
    result->value = floor(result->value);
    return result;
}

void *pyfloat__add__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        pyfloat *right = (pyfloat *)rvoid;
        pyfloat *retptr = pyfloat__init__();
        retptr->value = left->value + right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t)
        return pyint__add__(rvoid, lvoid);
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        retptr->real = pyfloat__add__(left, right->real);
        retptr->imag = right->imag;
        return retptr;
    }
}

void *pyfloat__sub__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        pyfloat *right = (pyfloat *)rvoid;
        pyfloat *retptr = pyfloat__init__();
        retptr->value = left->value - right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        pyfloat *retptr = pyfloat__sub__(lvoid, right);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        retptr->real = pyfloat__sub__(left, right->real);
        retptr->imag->value = -(right->imag->value);
        return retptr;
    }
}

pyfloat *pyfloat__neg__(pyfloat *ptr) {
    pyfloat *retptr = pyfloat__init__();
    retptr->value = -ptr->value;
    return retptr;
}

void *pyfloat__pow__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        void *retptr = pyfloat__pow__(lvoid, right);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t) {
        pyfloat *left = (pyfloat *)lvoid;
        pyfloat *right = (pyfloat *)rvoid;
        pyfloat *retptr = pyfloat__init__();
        retptr->value = pow(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *left = pyfloat__complex__(lvoid);
        void *retptr = pycomplex__pow__(left, rvoid);
        return retptr;
    }
}

/* not perfect yet, test 'int(434239988342466242342.43242342)' */
/* CAUTION: never forget to return */
pyint *pyfloat__int__(void *vptr) {
    pyfloat *ptr = (pyfloat *)vptr;
    double fvalue = ptr->value;
    char sign = '+';
    if (fvalue < 0) {
        fvalue = -fvalue;
        sign = '-';
    }
    int divider = pow(10, INTEGER_SZ);
    int part = (int)(fvalue - floor(fvalue / divider) * divider);
    int index = 0;
    integer *node = INTEGER_NODE(), *new_node;
    node->value = part;
    fvalue /= divider;
    while (fvalue >= 1.0) {
        part = (int)(fvalue - floor(fvalue / divider) * divider);
        new_node = INTEGER_NODE();
        new_node->value = part;
        new_node->index = ++index;
        fvalue /= divider;
        node->higher = new_node;
        new_node->lower = node;
        node = new_node;
    }
    node->sign = sign;
    pyint *retptr = pyint__init__();
    retptr->value = node;
    return retptr;
}

pyfloat *pyfloat__float__(void *vptr) {
    return (pyfloat *)vptr;
}

pycomplex *pyfloat__complex__(void *vptr) {
    pycomplex *retptr = pycomplex__init__();
    retptr->real->value = ((pyfloat *)vptr)->value;
    return retptr;
}

pystr *pyfloat__str__(void *vptr) {
    char value[1024];
    sprintf(value, "%f", ((pyfloat *)vptr)->value);
    return pystr_init2(value);
}

pyfloat *pyfloat__abs__(void *vptr) {
    pyfloat *retptr = pyfloat__init__();
    retptr->value = -((pyfloat *)vptr)->value;
    return retptr;
}

pyfloat *pyfloat__mod__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    pyfloat *retptr = pyfloat__init__();
    if (type(rvoid) == pyfloat_t) {
        pyfloat *right = (pyfloat *)rvoid;
        retptr->value = left->value - floor(left->value / right->value) * right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        retptr->value = left->value - floor(left->value / right->value) * right->value;
        return retptr;
    }
}

pybool *pyfloat__gt__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        if (left->value > ((pyfloat *)rvoid)->value)
            return PYBOOL(1);
        return PYBOOL(0);
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        return pyfloat__gt__(lvoid, right);
    }
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyfloat__gt__(lvoid, int_to_pyint(right->value));
    }
}

pybool *pyfloat__lt__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        if (left->value < ((pyfloat *)rvoid)->value)
            return PYBOOL(1);
        return PYBOOL(0);
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        return pyfloat__lt__(lvoid, right);
    }
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyfloat__lt__(lvoid, int_to_pyint(right->value));
    }
}


pybool *pyfloat__eq__(void *lvoid, void *rvoid) {
    pyfloat *left = (pyfloat *)lvoid;
    if (type(rvoid) == pyfloat_t) {
        if (left->value == ((pyfloat *)rvoid)->value)
            return PYBOOL(1);
        return PYBOOL(0);
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        return pyfloat__eq__(lvoid, right);
    }
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyfloat__eq__(lvoid, int_to_pyint(right->value));
    }
}