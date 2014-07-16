#ifndef PYTYPE_PYBOOL_H
#define PYTYPE_PYBOOL_H

#include "pyclass.h"

typedef struct pybool pybool;
struct pybool {
    int type;
    pyclass *class;
    int value;
};

void *PYBOOL(int);
pybool *pybool__bool__(void *);
void *pybool__gt__(void *lvoid, void *rvoid);
void *pybool__lt__(void *lvoid, void *rvoid);
void *pybool__eq__(void *lvoid, void *rvoid);

int is_true(pybool *);

#endif /* PYTYPE_PYBOOL_H */