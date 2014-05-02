#ifndef PYTYPE_PYCLASS_H
#define PYTYPE_PYCLASS_H

#include "../environment.h"
#include "pystr.h"

typedef struct pyclass pyclass;

struct pyclass {
    int type;
    pyclass *class;
    char *id;
    environment *env;
};

typedef struct instance instance;

struct instance {
    int type;
    pyclass *class;
    environment *env;
};

pyclass *pyclass__init__(char *id);
void *pyclass__getattribute__(void *, void *, pystr *);
void *pyclass__call__(void *, void *);
void pyclass__setattr__(void *, void *, pystr *attr, void *val);

#endif /* PYTYPE_PYCLASS_H */