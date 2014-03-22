#include "cpyter.h"

pylist *pylist__init__() {
	pylist *retptr = (pylist *)malloc(sizeof(pylist));
	retptr->type = pylist_t;
	return retptr;
}

void *pylist__sort__(pylist *val, int comp()) {
	list_sort(val->values, comp);
}