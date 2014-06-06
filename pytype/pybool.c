#include <stdlib.h>
#include "../types.h"
#include "pybool.h"

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
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
	return boolval->value;
}

void pybool_del(void *vptr) {
    free(vptr);
}