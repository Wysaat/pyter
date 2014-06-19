#ifndef PYTYPE_PYSET_H
#define PYTYPE_PYSET_H

#include "../list.h"
#include "pyint.h"

typedef struct pyset pyset;
struct pyset {
    int type;
    int ref;
    list *values;
};

pyint *pyset__len__(void *);

#endif /* PYTYPE_PYSET_H */