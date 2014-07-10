#ifndef PYTYPE_PYSET_H
#define PYTYPE_PYSET_H

#include "pyclass.h"
#include "../list.h"
#include "pyint.h"

typedef struct pyset pyset;
struct pyset {
    int type;
    pyclass *class;
    list *values;
};

pyset *pyset_init();
pyint *pyset__len__(void *);
pystr *pyset_str(void *);

#endif /* PYTYPE_PYSET_H */