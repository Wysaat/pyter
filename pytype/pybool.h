#ifndef PYTYPE_PYBOOL_H
#define PYTYPE_PYBOOL_H

#include "pytype.h"

typedef struct pybool pybool;
struct pybool {
    int type;
    int ref;
    pyclass *class;
    int value;
};

void *PYBOOL(int);
pybool *pybool__bool__(void *);
void pybool_del(void *);
void pybool_ref(void *);

int is_true(pybool *);

#endif /* PYTYPE_PYBOOL_H */