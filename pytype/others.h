#ifndef PYTYPE_OTHERS_H
#define PYTYPE_OTHERS_H

#include "pyint.h"
#include "../list.h"

typedef struct pyslice pyslice;
typedef struct pyNone pyNone;
typedef struct pyargument pyargument;

struct pyslice {
    int type;
    pyint *start;
    pyint *stop;
    pyint *step;
};

typedef struct pyNone pyNone;
struct pyNone {
    int type;
};

struct pyargument {
    list *assign_target_list;
    list *assign_value_list;
    list *value_list;
};

pyNone *pyNone_init();

#endif /* PYTYPE_OTHERS_H */