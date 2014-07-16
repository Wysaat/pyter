#ifndef PYTYPE_OTHERS_H
#define PYTYPE_OTHERS_H

#include "../list.h"
#include "pyclass.h"
#include "pyint.h"
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
    pyclass *class;
};

struct pyargument {
    int type;
    list *assign_target_list;  // can be 0
    list *assign_value_list;  // can be 0
    list *value_list;  // can be 0
};

struct pyrange {
    int type;
    pyclass *class;
    pyint *start;
    pyint *stop;
    pyint *step;
};

pyNone *pyNone_init();
pyrange *pyrange_init(void *);
pyrange *pyrange_init2(pyint *start, pyint *stop, pyint *step);
pyrange *pyrange_init3(int start, int stop, int step);
void *pyrange__getitem__(void *, void *);
void *pyrange_getitem2(void *lvoid, int pos);
pystr *pyrange__str__(void *vptr);
pyint *pyrange_len(void *vptr);
int pyrange_len2(void *vptr);
void pyargument_del(void *);
pyargument *pyargument_init();
pyargument *pyargument_init2(list *value_list);
pyargument *pyargument_init3(void *left, void *right);

#endif /* PYTYPE_OTHERS_H */