#ifndef PYTYPE_PYSET_H
#define PYTYPE_PYSET_H

#include "pyclass.h"
#include "../list.h"
#include "pyint.h"

typedef struct pyset pyset;
struct pyset {
    int type;
    int ref;
    pyclass *class;
    list *values;
};

pyint *pyset__len__(void *);
pystr *pyset_str(void *);

#endif /* PYTYPE_PYSET_H */