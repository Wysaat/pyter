#ifndef PYTYPE_PYLIST_H
#define PYTYPE_PYLIST_H

#include "pyclass.h"
#include "../list.h"
#include "pybool.h"
#include "pyint.h"

typedef struct pylist pylist;

struct pylist {
    int type;
    pyclass *class;
    list *values;
};

pylist *pylist__init__();
pylist *pylist_init2(list *values);
pylist *pylist__add__(void *, void *);
void pylist__append__(pylist *, void *);
pylist *pylist__mul__(void *, void *);
void pylist__sort__(pylist *, int comp());
pybool *pylist__eq__(void *, void *);
void *pylist__getitem__(void *, void *);
void pylist__setitem__(void *, void *, void *value);
pyint *pylist__len__(void *);
int pylist_len2(void *);
pystr *pylist_str(void *);

#endif /* PYTYPE_PYLIST_H */