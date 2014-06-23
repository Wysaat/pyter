#ifndef PYTYPE_PYFLOAT_H
#define PYTYPE_PYFLOAT_H

#include "pyclass.h"
#include "pyint.h"
#include "pycomplex.h"
#include "pystr.h"

typedef struct pyfloat pyfloat;
struct pyfloat {
    int type;
    int ref;
    pyclass *class;
    double value;
};

pyfloat *pyfloat__init__();
void pyfloat__del__(void *);
void *pyfloat__mul__(void *, void *);
void *pyfloat__div__(void *, void *);
void *pyfloat__rfloordiv__(void *, void *);
void *pyfloat__add__(void *, void *);
void *pyfloat__sub__(void *, void *);
pyfloat *pyfloat__neg__(pyfloat *);
pyint *pyfloat__int__(void *);

void *pyfloat__pow__(void *, void *);

pyint *pyfloat__int__(void *);
pyfloat *pyfloat__float__(void *);
pycomplex *pyfloat__complex__(void *);
pystr *pyfloat__str__(void *);

#endif /* PYTYPE_PYFLOAT_H */