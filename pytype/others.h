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
    pyint *start;
    pyint *stop;
    pyint *step;
};

struct pyNone {
    int type;
};

struct pyargument {
    list *assign_target_list;
    list *assign_value_list;
    list *value_list;
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

#endif /* PYTYPE_OTHERS_H */