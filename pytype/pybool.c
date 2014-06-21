#include <stdlib.h>
#include "../types.h"
#include "pybool.h"

void *PYBOOL(int value) {
    pybool *retptr = (pybool *)malloc(sizeof(pybool));
    retptr->type = pybool_t;
    retptr->ref = 0;
    retptr->type = &bool_class;
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
    ref_dec(vptr);
    if (get_ref(vptr) == 0)
        free(vptr);
}

void pybool_ref(void *vptr) {
    ref_inc(vptr);
}