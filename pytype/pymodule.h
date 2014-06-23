#ifndef PYTYPE_PYMODULE_H
#define PYTYPE_PYMODULE_H

#include "pyclass.h"
#include "../environment.h"

typedef struct pymodule pymodule;

struct pymodule {
    int type;
    int ref;
    pyclass *class;
    char *name;
    environment *env;
};

pymodule *pymodule_init(char *name);

#endif /* PYTYPE_PYMODULE_H */