#include "pyset.h"
#include "pyint.h"

pyint *pyset__len__(void *vptr) {
    pyset *ptr = (pyset *)vptr;
    return int_to_pyint(list_len(ptr->values));
}