#ifndef PYTYPE_PYTUPLE_H
#define PYTYPE_PYTUPLE_H

#include "pyclass.h"
#include "../list.h"
#include "pyint.h"
#include "pybool.h"

typedef struct pytuple pytuple;
struct pytuple {
    int type;
    pyclass *class;
    list *values;
};

pytuple *pytuple__init__();
pytuple *pytuple__add__(void *, void *);
pytuple *pytuple__mul__(pytuple *, void *);
pybool *pytuple__eq__(void *, void *);
void *pytuple__getitem__(void *, void *);
pyint *pytuple__len__(void *);
pystr *pytuple_str(void *);

#endif /* PYTYPE_PYTUPLE_H */