#ifndef PYTYPE_OTHERS_H
#define PYTYPE_OTHERS_H

#include "pyint.h"

typedef struct pyslice pyslice;
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

pyNone *pyNone_init();

#endif /* PYTYPE_OTHERS_H */