#include <stdlib.h>
#include "others.h"
#include "../types.h"

pyNone *pyNone_init() {
    pyNone *retptr = (pyNone *)malloc(sizeof(pyNone));
    retptr->type = pyNone_t;
    return retptr;
}