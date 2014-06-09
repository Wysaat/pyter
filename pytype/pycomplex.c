#include "pycomplex.h"
#include "../types.h"
#include <stdlib.h>
#include "pyint.h"

pycomplex *pycomplex__init__() {
    pycomplex *retptr = (pycomplex *)malloc(sizeof(pycomplex));
    retptr->type = pycomplex_t;
    retptr->real = pyfloat__init__();
    retptr->imag = pyfloat__init__();
    return retptr;
}

pycomplex *pycomplex__add__(void *lvoid, void *rvoid) {
    pycomplex *left = (pycomplex *)lvoid;
    if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        pyfloat__del__(retptr->real);
        pyfloat__del__(retptr->imag);
        retptr->real = pyfloat__add__(left->real, right->real);
        retptr->imag = pyfloat__add__(left->imag, right->imag);
        return retptr;
    }
    else if (type(rvoid) == pyint_t)
        return pyint__add__(rvoid, lvoid);
    else if (type(rvoid) == pyfloat_t)
        return pyfloat__add__(rvoid, lvoid);
}

pycomplex *pycomplex__sub__(void *lvoid, void *rvoid) {
    pycomplex *left = (pycomplex *)lvoid;
    if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        pyfloat__del__(retptr->real);
        pyfloat__del__(retptr->imag);
        retptr->real = pyfloat__sub__(left->real, right->real);
        retptr->imag = pyfloat__sub__(left->imag, right->imag);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pyint_t) {
        pycomplex *retptr = pycomplex__init__();
        pyfloat__del__(retptr->real);
        pyfloat__del__(retptr->imag);
        retptr->real = pyfloat__sub__(left->real, rvoid);
        retptr->imag = left->imag;
        return retptr;
    }
}

pycomplex *pycomplex__mul__(void *lvoid, void *rvoid) {
    pycomplex *left = (pycomplex *)lvoid;
    if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        pyfloat__del__(retptr->real);
        pyfloat__del__(retptr->imag);
        retptr->real = pyfloat__sub__(pyfloat__mul__(left->real, right->real),
                                      pyfloat__mul__(left->imag, right->imag));
        retptr->imag = pyfloat__add__(pyfloat__mul__(left->real, right->imag),
                                      pyfloat__mul__(left->imag, right->real));
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pyint_t) {
        pycomplex *retptr = pycomplex__init__();
        pyfloat__del__(retptr->real);
        pyfloat__del__(retptr->imag);
        retptr->real = pyfloat__mul__(left->real, rvoid);
        retptr->imag = pyfloat__mul__(left->imag, rvoid);
        return retptr;
    }
}

pycomplex *pycomplex__div__(void *lvoid, void *rvoid) {
    pycomplex *left = (pycomplex *)lvoid;
    if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *x = pycomplex__init__();
        x->real->value = right->real->value;
        x->imag->value = -(right->imag->value);
        pycomplex *retptr = pycomplex__mul__(left, x);
        pycomplex *y = pycomplex__mul__(right, x);
        retptr->real->value /= y->real->value;
        retptr->imag->value /= y->real->value;
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t) {
        pycomplex *retptr = pycomplex__init__();
        pyfloat *right = (pyfloat *)rvoid;
        retptr->real->value = left->real->value / right->value;
        retptr->imag->value = left->imag->value / right->value;
        return retptr;
    }
    else if (type(rvoid) == pyint_t) {
        pyfloat *fright = pyint__float__(rvoid);
        pycomplex *retptr = pycomplex__div__(lvoid, fright);
        pyfloat__del__(fright);
        return retptr;
    }
}

pycomplex *pycomplex__neg__(pycomplex *ptr) {
    pycomplex *retptr = pycomplex__init__();
    retptr->real = pyfloat__neg__(ptr->real);
    retptr->imag = pyfloat__neg__(ptr->imag);
    return retptr;
}