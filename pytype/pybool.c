#include <stdlib.h>
#include "../types.h"
#include "pybool.h"

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
    retptr->value = value;
    return retptr;
}

int is_true(pybool *boolval) {
	return boolval->value;
}