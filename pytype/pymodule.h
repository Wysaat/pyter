#ifndef PYTYPE_PYMODULE_H
#define PYTYPE_PYMODULE_H

#include "../environment.h"
typedef struct environment environment;

typedef struct pymodule pymodule;

struct pymodule {
    int type;
    int ref;
    char *name;
    environment *env;
};

pymodule *pymodule_init(char *name);

#endif /* PYTYPE_PYMODULE_H */