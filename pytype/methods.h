#ifndef PYTYPE_METHODS_H
#define PYTYPE_METHODS_H

#include "../environment.h"
#include "../types.h"
#include "../evaluate.h"
#include "pyint.h"
#include "pyfloat.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pybool.h"
#include "pylist.h"
#include "pytuple.h"
#include "pyset.h"
#include "pydict.h"
#include "pyfunction.h"
#include "pyclass.h"
#include "py__builtins__.h"
#include "others.h"

pybool *__eq__(void *, void *);
void *__getitem__(void *, void *);
void __setitem__(void *, void *, void *);
void *__mul__(void *, void *);
void *__div__(void *, void *);
void *__rfloordiv__(void *, void *);
void *__add__(void *, void *);
void *__sub__(void *, void *);
pybool *__bool__(void *);
void *__call__(void *, void *);
void *__getattribute__(void *, void *, pystr *);
void __setattr__(void *, void *, char *, void *);
pyint *len(void *);
pystr *str(void *vptr);
pyint *__int__(void *);
pyfloat *__float__(void *);
pycomplex *py__complex__(void *);
pylist *__list__(void *);
pytuple *__tuple__(void *);
void *__pow__(void *, void *);
void *__abs__(void *);

#endif /* PYTYPE_METHODS_H */