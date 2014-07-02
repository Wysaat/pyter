#ifndef PYTYPE_PYINT_H
#define PYTYPE_PYINT_H

#include "pyclass.h"
#include "../integer.h"
#include "pybool.h"

typedef struct pyfloat pyfloat;
typedef struct pycomplex pycomplex;

typedef struct pyint pyint;

struct pyint {
    int type;
    int ref;
    pyclass *class;
    integer *value;
};

pyint *pyint__init__();
pyint *pyint_init2(void *, pyint *);
void pyint__del__(void *);
void pyint_del2(void *);
void pyint_ref(void *);
pybool *pyint__bool__(void *);
void *pyint__add__(void *, void *);
void *pyint_add2(void *, void *);    // left deleted
void *pyint_add3(void *, void *);    // left and right both deleted
void *pyint__sub__(void *, void *);
void *pyint__mul__(void *, void *);
void *pyint_mul2(void *, void *);    // left deleted
void *pyint_mul3(void *, void *);    // left and right both deleted
void *pyint__div__(void *, void *);
void *pyint__rfloordiv__(void *, void *);
void *pyint__mod__(void *, void *);
pyint *pyint__lshift__(pyint *, pyint *);
pyint *pyint__rshift__(pyint *, pyint *);
pybool *pyint__lt__(void *, void *);
pybool *pyint__gt__(void *, void *);
int pyint__cmp__(pyint *, pyint *);
pybool *pyint__eq__(void *, void *);
pybool *pyint__ge__(void *, void *);

void pyint__print__(pyint *);
void pyint__puts__(pyint *);

void pyint__dec__(pyint *);
void pyint__inc__(pyint *);

pyint *int_to_pyint(int );
int pyint_to_int(pyint *);
pyint *pyint_cpy(void *);

pyint *pyint__neg__(pyint *);
pyint *pyint__invert__(pyint *);

void *pyint__pow__(void *, void *);

pyint *pyint__int__(void *);
pyfloat *pyint__float__(void *);
pycomplex *pyint__complex__(void *);
pystr *pyint__str__(void *);
pyint *pyint__abs__(void *);

#endif /* PYTYPE_PYINT_H */