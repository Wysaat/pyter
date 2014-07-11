#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pycomplex.h"
#include "../types.h"
#include "pyfloat.h"
#include "../struct_info.h"
#include "../environment.h"
#include "pyint.h"
#include "pystr.h"
#include "methods.h"
#include "../builtins/builtins.h"

pycomplex *pycomplex__init__() {
    pycomplex *retptr = (pycomplex *)malloc(sizeof(pycomplex));
    retptr->type = pycomplex_t;
    retptr->class = &complex_class;
    retptr->real = pyfloat__init__();
    retptr->imag = pyfloat__init__();
    return retptr;
}

pycomplex *pycomplex__add__(void *lvoid, void *rvoid) {
    pycomplex *left = (pycomplex *)lvoid;
    if (type(rvoid) == pycomplex_t) {
        pycomplex *right = (pycomplex *)rvoid;
        pycomplex *retptr = pycomplex__init__();
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
        retptr->real = pyfloat__sub__(left->real, right->real);
        retptr->imag = pyfloat__sub__(left->imag, right->imag);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pyint_t) {
        pycomplex *retptr = pycomplex__init__();
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
        retptr->real = pyfloat__sub__(pyfloat__mul__(left->real, right->real),
                                      pyfloat__mul__(left->imag, right->imag));
        retptr->imag = pyfloat__add__(pyfloat__mul__(left->real, right->imag),
                                      pyfloat__mul__(left->imag, right->real));
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pyint_t) {
        pycomplex *retptr = pycomplex__init__();
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
        return retptr;
    }
}

pycomplex *pycomplex__neg__(pycomplex *ptr) {
    pycomplex *retptr = pycomplex__init__();
    retptr->real = pyfloat__neg__(ptr->real);
    retptr->imag = pyfloat__neg__(ptr->imag);
    return retptr;
}

void *pycomplex__pow__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t || type(rvoid) == pyfloat_t) {
        pycomplex *right = py__complex__(rvoid);
        void *retptr = pycomplex__pow__(lvoid, right);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        // e**(i*x) = cos(x) + i*sin(x)
        // x ** i = (e**(ln(x)*i)) = cos(ln(x)) + i*sin(ln(x))
        double e = 2.718281828459045;
        pycomplex *left = (pycomplex *)lvoid;
        pycomplex *right = (pycomplex *)rvoid;
        double a, b, c, d, left_abs, t, rp, cp;
        a = left->real->value;
        b = left->imag->value;
        c = right->real->value;
        d = right->imag->value;
        left_abs = pycomplex__abs__(left)->value;
        t = acos(a / sqrt(pow(a, 2)+pow(b, 2)));
        if (b < 0)
            t = -t;
        rp = pow(left_abs, c) * pow(e, -(d * t));
        cp = pow(left_abs, d) * pow(e, c * t);
        pycomplex *retptr = pycomplex__init__();
        retptr->real->value = cos(log(cp)) * rp;
        retptr->imag->value = sin(log(cp)) * rp;
        return retptr;
    }
}

pyfloat *pycomplex__abs__(void *vptr) {
    pycomplex *ptr = (pycomplex *)vptr;
    pyfloat *retptr = pyfloat__init__();
    retptr->value = sqrt(ptr->real->value * ptr->real->value + ptr->imag->value * ptr->imag->value);
    return retptr;
}

pycomplex *pycomplex__complex__(void *vptr) {
    return (pycomplex *)vptr;
}

pystr *pycomplex__str__(void *vptr) {
    pycomplex *ptr = (pycomplex *)vptr;
    char value[1024];
    sprintf(value, "(%f+%fj)", ptr->real->value, ptr->imag->value);
    return pystr_init2(value);
}
