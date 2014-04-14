#ifndef PYTYPE_PYBOOL_H
#define PYTYPE_PYBOOL_H

typedef struct pybool pybool;
struct pybool {
    int type;
    int value;
};

void *PYBOOL(int);
pybool *pybool__bool__(void *);

int is_true(pybool *);

#endif /* PYTYPE_PYBOOL_H */