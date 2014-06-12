#ifndef PYTYPE_PYINT_H
#define PYTYPE_PYINT_H

#include "../integer.h"
#include "pybool.h"
#include "pyfloat.h"

typedef struct pyint pyint;

struct pyint {
    int type;
    int ref;
    integer *value;
};

pyint *pyint__init__();
void pyint__del__(void *);
void pyint_ref(void *);
pyfloat *pyint__float__(void *);
pybool *pyint__bool__(void *);
void *pyint__add__(void *, void *);
void *pyint__sub__(void *, void *);
void *pyint__mul__(void *, void *);
void *pyint__div__(void *, void *);
pyint *pyint__mod__(pyint *, pyint *);
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

#endif /* PYTYPE_PYINT_H */