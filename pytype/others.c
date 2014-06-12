#include <stdlib.h>
#include "others.h"
#include "../types.h"
#include "pyint.h"
#include "pystr.h"
#include "../string.h"

pyNone *pyNone_init() {
    pyNone *retptr = (pyNone *)malloc(sizeof(pyNone));
    retptr->type = pyNone_t;
    return retptr;
}

pyrange *pyrange_init(void *vptr) {
    pyrange *retptr = (pyrange *)malloc(sizeof(pyrange));
    pyargument *argument = (pyargument *)vptr;
    list *value_list = argument->value_list;
    if (list_len(value_list) == 1) {
        retptr->start = int_to_pyint(0);
        retptr->stop = value_list->content;
        retptr->step = int_to_pyint(1);
    }
    else if (list_len(value_list) == 2) {
        retptr->start = value_list->content;
        retptr->stop = value_list->next->content;
        retptr->step = int_to_pyint(1);
    }
    else {
        retptr->start = value_list->content;
        retptr->stop = value_list->next->content;
        retptr->step = value_list->next->next->content;
    }
    retptr->type = pyrange_t;
    return retptr;
}

void *pyrange__getitem__(void *lvoid, void *rvoid) {
    pyrange *range = (pyrange *)lvoid;
    if (type(rvoid) == pyint_t)
        return pyint__add__(range->start, pyint__mul__(range->step, rvoid));
}

pystr *pyrange__str__(void *vptr) {
    pyrange *range = (pyrange *)vptr;
    char *start = integer__str__(range->start->value);
    char *stop = integer__str__(range->stop->value);
    char *step = integer__str__(range->step->value);
    char *string;
    if (pyint_to_int(range->step) == 1)
        string = stradd(stradd("range(", stradd(stradd(start, ", "), stop)), ")");
    else
        string = stradd(stradd("range(", stradd(stradd(stradd(stradd(start, ", "), stop), ", "), step)), ")");
    return str_to_pystr(string);
}

void pyargument_del(void *vptr) {
    pyargument *ptr = (pyargument *)vptr;
    if (ptr->assign_target_list)
        del(ptr->assign_target_list);
    if (ptr->assign_value_list)
        del(ptr->assign_value_list);
    if (ptr->value_list)
        del(ptr->value_list);
    free(ptr);
}
