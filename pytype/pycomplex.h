#include "pyfloat.h"

typedef struct pycomplex pycomplex;
struct pycomplex {
    int type;
    pyfloat *real;
    pyfloat *imag;
};

pycomplex *pycomplex__init__();
pycomplex *pycomplex__add__(void *, void *);
pycomplex *pycomplex__sub__(void *, void *);
pycomplex *pycomplex__mul__(void *, void *);
pycomplex *pycomplex__div__(void *, void *);