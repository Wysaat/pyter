#ifndef PYTYPE_PYLIST_H
#define PYTYPE_PYLIST_H

#include "../list.h"
#include "pybool.h"

typedef struct pylist pylist;

struct pylist {
    int type;
    list *values;
};

pylist *pylist__init__();
pylist *pylist__add__(void *, void *);
void pylist__append__(pylist *, void *);
pylist *pylist__mul__(void *, void *);
void pylist__sort__(pylist *, int comp());
pybool *pylist__eq__(void *, void *);
void *pylist__getitem__(void *, void *);
void pylist__setitem__(void *, void *, void *value);

#endif /* PYTYPE_PYLIST_H */