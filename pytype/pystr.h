#ifndef PYTYPE_PYSTR_H
#define PYTYPE_PYSTR_H

#include "pybool.h"

typedef struct pystr pystr;
struct pystr {
    int type;
    char *value;
};

pystr *pystr__init__();
void pystr__del__(pystr *);
pystr *pystr__mul__(void *, void *);
pystr *pystr__add__(void *, void *);
pybool *pystr__eq__(void *, void *);
pybool *pystr__bool__(void *);

void pystr__print__(pystr *);
void pystr__puts__(pystr *);

pystr *str_to_pystr(char *);
char *pystr_to_str(pystr *ptr);

#endif /* PYTYPE_PYSTR_H */