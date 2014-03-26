#ifndef PYTYPE_PYINT_H
#define PYTYPE_PYINT_H

#include "../integer.h"

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
int pyint__cmp__(pyint *, pyint *);



pyint *pyint__dec__(pyint *);
pyint *pyint__inc__(pyint *);

#endif /* PYTYPE_PYINT_H */