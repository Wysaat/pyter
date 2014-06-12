#ifndef PYTYPE_PYCOMPLEX_H
#define PYTYPE_PYCOMPLEX_H

#include "pyfloat.h"

typedef struct pycomplex pycomplex;
struct pycomplex {
    int type;
    int ref;
    pyfloat *real;
    pyfloat *imag;
};

pycomplex *pycomplex__init__();
pycomplex *pycomplex__add__(void *, void *);
pycomplex *pycomplex__sub__(void *, void *);
pycomplex *pycomplex__mul__(void *, void *);
pycomplex *pycomplex__div__(void *, void *);
pycomplex *pycomplex__neg__(pycomplex *);
void pycomplex_del(void *);
void pycomplex_ref(void *);

#endif /* PYTYPE_PYCOMPLEX_H */