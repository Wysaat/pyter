#ifndef PYTYPE_PYSTR_H
#define PYTYPE_PYSTR_H

#include "pyclass.h"
#include "pyint.h"
#include "pybool.h"

typedef struct pystr pystr;
struct pystr {
    int type;
    int ref;
    pyclass *class;
    char *value;
};

pystr *pystr__init__();
pystr *pystr_init2(char *value); // value is strduped
void pystr__del__(void *);
void pystr_ref(void *);
pystr *pystr__mul__(void *, void *);
pystr *pystr__add__(void *, void *);
pybool *pystr__eq__(void *, void *);
pybool *pystr__bool__(void *);

void pystr__print__(pystr *);
void pystr__puts__(pystr *);

pystr *str_to_pystr(char *);
char *pystr_to_str(pystr *ptr);

void pystr_print_nnl(pystr *ptr);
pyint *pystr_len(void *);
pystr *pystr__getitem__(void *, void *);

pystr *pystr__str__(void *);

pystr *pystr_capitalize(pystr *ptr);
pystr *pystr_casefold(pystr *ptr);
pystr *pystr_center(pystr *ptr, pyint *width, pystr *fillchar);
pyint *pystr_count(pystr *ptr, pystr *sub, pyint *start, pyint *end);
pybool *pystr_endswith(pystr *ptr, pystr *suffix, pyint *start, pyint *end);

#endif /* PYTYPE_PYSTR_H */