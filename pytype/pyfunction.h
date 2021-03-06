#ifndef PYTYPE_PYFUNCTION_H
#define PYTYPE_PYFUNCTION_H

#include "pyclass.h"
#include "../evaluate.h"
#include "../list.h"
#include "../environment.h"
#include "pytuple.h"

typedef struct pyfunction pyfunction;

struct pyfunction {
    int type;
    pyclass *class;
    identifier *id;
    list *parameters;  // can be 0, or a list of identifiers
    void *fsuite;
    environment *env;
    void *bound;
    int yield;  /* for generators */
    expression_list *assign_target_list;  // can be 0
    pytuple *assign_values;  // can be 0
};

pyfunction *pyfunction_init();
void *pyfunction__call__(void *, void *);;

#endif /* PYTYPE_PYFUNCTION_H */