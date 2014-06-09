#ifndef PYTYPE_PYFLOAT_H
#define PYTYPE_PYFLOAT_H

typedef struct pyfloat pyfloat;
struct pyfloat {
    int type;
    int ref;
    double value;
};

pyfloat *pyfloat__init__();
void pyfloat__del__(void *);
void *pyfloat__mul__(void *, void *);
void *pyfloat__div__(void *, void *);
void *pyfloat__add__(void *, void *);
void *pyfloat__sub__(void *, void *);
pyfloat *pyfloat__neg__(pyfloat *);

#endif /* PYTYPE_PYFLOAT_H */