#ifndef PYTYPE_METHODS_H
#define PYTYPE_METHODS_H

#include "pybool.h"
#include "pystr.h"
#include "../environment.h"
#include "others.h"

pybool *__eq__(void *, void *);
void *__getitem__(void *, void *);
void __setitem__(void *, void *, void *);
void *__mul__(void *, void *);
void *__div__(void *, void *);
void *__add__(void *, void *);
void *__sub__(void *, void *);
pybool *__bool__(void *);
void *__call__(void *, void *);
void *__getattribute__(void *, void *, pystr *);
void __setattr__(void *, void *, pystr *, void *);
pyint *len(void *);

#endif /* PYTYPE_METHODS_H */