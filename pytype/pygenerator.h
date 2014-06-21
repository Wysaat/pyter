#ifndef PYTYPE_PYGENERATOR_H
#define PYTYPE_PYGENERATOR_H

#include "../environment.h"

typedef struct pygenerator pygenerator;

struct pygenerator {
    int type;
    int ref;
    void *_suite;
    environment *local_env;
    char *id;
};

pygenerator *pygenerator_init(void *_suite, environment *local_env, char *id);
pygenerator *pygenerator_next(void *);
void pygenerator_del(void *);

#endif /* PYTYPE_PYGENERAOTR_H */