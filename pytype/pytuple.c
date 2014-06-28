#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "../struct_info.h"
#include "../__builtins__.h"
#include "pytuple.h"
#include "pyint.h"
#include "pybool.h"
#include "pystr.h"
#include "methods.h"
#include "../list.h"
#include "../string.h"

pytuple *pytuple__init__() {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
    retptr->ref = 0;
    retptr->class = &tuple_class;
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

    pyint *ind = int_to_pyint(0);

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
            del(lptr->content);
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
            ref(lptr->content);
    }
}

pyint *pytuple__len__(void *vptr) {
    pytuple *ptr = (pytuple *)vptr;
    return int_to_pyint(list_len(ptr->values));
}

pystr *pytuple_str(void *vptr) {
    pytuple *tptr = (pytuple *)vptr;
    buffer2 *buff = buff2_init();
    buff2_add(buff, strdup("("));
    list *ptr;
    for (ptr = tptr->values; ptr->next; ptr = ptr->next) {
        buff2_add(buff, str(ptr->content)->value);
        buff2_add(buff, strdup(", "));
    }
    buff2_add(buff, str(ptr->content)->value);
    buff2_add(buff, strdup(")"));
    return pystr_init3(buff2_puts2(buff));
}