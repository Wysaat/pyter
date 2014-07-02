#ifndef PYTYPE_METHODS_H
#define PYTYPE_METHODS_H

typedef struct pybool pybool;
typedef struct pyint pyint;
typedef struct pystr pystr;
typedef struct pyfloat pyfloat;
typedef struct pycomplex pycomplex;
typedef struct pylist pylist;
typedef struct pytuple pytuple;

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
void *__getattribute__(void *, void *, char *);
void __setattr__(void *, void *, char *, void *);
pyint *len(void *);
pystr *str(void *vptr);
pyfloat *__float__(void *);
pycomplex *py__complex__(void *);
pylist *__list__(void *);
pytuple *__tuple__(void *);
void *__pow__(void *, void *);
void *__abs__(void *);
void *_mod_(void *, void *);

#endif /* PYTYPE_METHODS_H */