#include <stdlib.h>
#include "pybool.h"
#include "../types.h"
#include "../struct_info.h"
#include "../builtins/builtins.h"

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
    retptr->class = &bool_class;
    if (value)
        retptr->value = 1;
    else
        retptr->value = 0;
    return retptr;
}

pybool *pybool__bool__(void *ptr) {
    pybool *boolptr = (pybool *)ptr;
    return PYBOOL(boolptr->value);
}


int is_true(pybool *boolval) {
    int retval = boolval->value;
}
