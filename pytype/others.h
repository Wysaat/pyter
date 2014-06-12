#ifndef PYTYPE_OTHERS_H
#define PYTYPE_OTHERS_H

#include "pyint.h"
#include "../list.h"
#include "pystr.h"

typedef struct pyslice pyslice;
typedef struct pyNone pyNone;
typedef struct pyargument pyargument;
typedef struct pyrange pyrange;

struct pyslice {
    int type;
    int start;
    int stop;
    int step;
    int nostart; // e.g. a[::-1]
    int nostop; // e.g. a[2:] 
};

struct pyNone {
    int type;
};

struct pyargument {
    int type;
    list *assign_target_list;  // can be 0
    list *assign_value_list;  // can be 0
    list *value_list;  // can be 0
};

struct pyrange {
    int type;
    pyint *start;
    pyint *stop;
    pyint *step;
};

pyNone *pyNone_init();
pyrange *pyrange_init(void *);
void *pyrange__getitem__(void *, void *);
pystr *pyrange__str__(void *vptr);
void pyargument_del(void *);

#endif /* PYTYPE_OTHERS_H */