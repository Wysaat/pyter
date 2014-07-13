#ifndef PYTYPE_PYFLOAT_H
#define PYTYPE_PYFLOAT_H

#include "pyclass.h"
#include "pyint.h"
#include "pycomplex.h"
#include "pystr.h"

typedef struct pyfloat pyfloat;
struct pyfloat {
    int type;
    pyclass *class;
    double value;
};

pyfloat *pyfloat__init__();
pyfloat *pyfloat_init2(double );
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
pyfloat *pyfloat__abs__(void *);
pyfloat *pyfloat__mod__(void *, void *);
pybool *pyfloat__gt__(void *lvoid, void *rvoid);
pybool *pyfloat__lt__(void *lvoid, void *rvoid);

#endif /* PYTYPE_PYFLOAT_H */