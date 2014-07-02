#ifndef PYTYPE_PYCLASS_H
#define PYTYPE_PYCLASS_H

#include "../environment.h"
#include "../list.h"

typedef struct pystr pystr;

typedef struct pyclass pyclass;

struct pyclass {
    int type;
    int ref;
    pyclass *class;
    char *id;
    environment *env;
    list *inheritance;  // can be zero
};

typedef struct instance instance;

struct instance {
    int type;
    int ref;
    pyclass *class;
    environment *env;
};

pyclass *pyclass__init__(char *id);
void *pyclass__getattribute__(void *, void *, char *);
void *pyclass__call__(void *, void *);
void pyclass__setattr__(void *, void *, char *attr, void *val);
void pyclass_del(void *);
void pyclass_ref(void *);
instance *instance_init(pyclass *class);

#endif /* PYTYPE_PYCLASS_H */