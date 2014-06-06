#ifndef PYTYPE_PYFUNCTION_H
#define PYTYPE_PYFUNCTION_H

#include "../evaluate.h"
#include "../list.h"
#include "../environment.h"
#include "pytuple.h"

typedef struct pyfunction pyfunction;

struct pyfunction {
    int type;
    int ref;
    identifier *id;
    list *parameters;
    void *fsuite;
    environment *env;
    void *bound;
    int yield;  /* for generators */
    expression_list *assign_target_list;
    pytuple *assign_values;
};

void *pyfunction__call__(void *, void *);
void pyfunction_del(void *);

#endif /* PYTYPE_PYFUNCTION_H */