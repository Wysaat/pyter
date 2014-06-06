#ifndef PYTYPE_PYBOOL_H
#define PYTYPE_PYBOOL_H

typedef struct pybool pybool;
struct pybool {
    int type;
    int ref;
    int value;
};

void *PYBOOL(int);
pybool *pybool__bool__(void *);
void pybool_del(void *);

int is_true(pybool *);

#endif /* PYTYPE_PYBOOL_H */