#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../types.h"
#include "pyfloat.h"
#include "pycomplex.h"
#include "pyint.h"

pyfloat *pyfloat__init__() {
    pyfloat *retptr = (pyfloat *)malloc(sizeof(pyfloat));
    retptr->type = pyfloat_t;
    retptr->value = (double)0.0;
    return retptr;
}

void pyfloat__del__(void *vptr) {
    pyfloat *fptr = (pyfloat *)vptr;
    free(fptr);
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
        pyfloat__del__(fright);
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
        pyfloat__del__(fright);
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
        free(retptr->real);
        free(retptr->imag);
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
        pyfloat__del__(right);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
        free(retptr->real);
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

// pyint *pyfloat__int__(void *vptr) {
//     pyfloat *fptr = (pyfloat *)vptr;
//     double *fvalue;
//     if (fptr->value >= 0)
//         fvalue = floor(fptr->value);
//     else
//         fvalue = ceil(fptr->value);
//     while (1) {
//     }
// }

void *pyfloat__pow__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyfloat *right = pyint__float__(rvoid);
        void *retptr = pyfloat__pow__(lvoid, right);
        del(right);
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
        del(left);
        return retptr;
    }
}

// INTEGER_SZ
pyint *pyfloat__int__(void *vptr) {
    // pyfloat *ptr = (pyfloat *)vptr;
    // double *value = ptr->value;
    // while (1) {
    // }
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