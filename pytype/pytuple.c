#include <stdlib.h>
#include "../types.h"
#include "pytuple.h"
#include "pyint.h"
#include "pybool.h"
#include "../list.h"

pytuple *pytuple__init__() {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    retptr->values = list_node();
    return retptr;
}

pytuple *pytuple__add__(void *lvoid, void *rvoid) {
    pytuple *left = (pytuple *)lvoid;
    pytuple *right = (pytuple *)rvoid;
    pytuple *retptr = pytuple__init__();
    list_append_list(retptr->values, list_add(left->values, right->values));
    return retptr;
}

pytuple *pytuple__mul__(pytuple *left, void *right) {
    pytuple *retptr = pytuple__init__();
    pyint *times = (pyint *)right;
    pyint *zero = pyint__init__();
    zero->value = INTEGER_NODE();

    while (is_true(pyint__gt__(times, zero))) {
        list *to_append = list_cpy(left->values);
        list_append_list(retptr->values, to_append);
        pyint__dec__(times);
    }
    pyint__del__(zero);

    return retptr;
}

pybool *pytuple__eq__(void *lvoid, void *rvoid) {
    pytuple *left, *right;
    left = (pytuple *)lvoid;
    right = (pytuple *)rvoid;
    return PYBOOL(list_eq(left->values, right->values));
}

void *pytuple__getitem__(void *lvoid, void *rvoid) {
    pytuple *left = (pytuple *)lvoid;
    pyint *right = (pyint *)rvoid;
    list *ptr;

    pyint *ind = pyint__init__();
    ind->value = INTEGER_NODE();

    for (ptr = left->values; is_true(pyint__lt__(ind, right)); pyint__inc__(ind))
        ptr = ptr->next;
    return ptr->content;
}

void pytuple_del(void *vptr) {
    ref_dec(vptr);
    pytuple *ptr = (pytuple *)vptr;
    if (!list_is_empty(ptr->values)) {
        list *lptr;
        for (lptr = ptr->values; lptr; lptr = lptr->next)
            ref_dec(lptr->content);
    }
    if (get_ref(vptr) == 0) {
        list *lptr = ptr->values, *tmp;
        while (lptr) {
            tmp = lptr;
            lptr = lptr->next;
            free(tmp);
        }
        free(ptr);
    }
}

void pytuple_ref(void *vptr) {
    ref_inc(vptr);
    pytuple *ptr = (pytuple *)vptr;
    if (!list_is_empty(ptr->values)) {
        list *lptr;
        for (lptr = ptr->values; lptr; lptr = lptr->next)
            ref_inc(lptr->content);
    }
}

pyint *pytuple__len__(void *vptr) {
    pytuple *ptr = (pytuple *)vptr;
    return int_to_pyint(list_len(ptr->values));
}