#ifndef PYTYPE_PYINT_H
#define PYTYPE_PYINT_H

#include "../integer.h"
#include "pybool.h"

typedef struct pyint pyint;

struct pyint {
    int type;
    integer *value;
};

pyint *pyint__init__();
void pyint__del__(pyint *);
pyint *pyint__add__(pyint *, pyint *);
pyint *pyint__sub__(pyint *, pyint *);
pyint *pyint__mul__(pyint *, pyint *);
pyint *pyint__div__(pyint *, pyint *);
pyint *pyint__mod__(pyint *, pyint *);
pyint *pyint__lshift__(pyint *, pyint *);
pyint *pyint__rshift__(pyint *, pyint *);
pybool *pyint__lt__(pyint *, pyint *);
pybool *pyint__gt__(pyint *, pyint *);
int pyint__cmp__(pyint *, pyint *);

void pyint__print__(pyint *);
void pyint__puts__(pyint *);

void pyint__dec__(pyint *);
void pyint__inc__(pyint *);

#endif /* PYTYPE_PYINT_H */