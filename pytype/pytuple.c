#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "../struct_info.h"
#include "pytuple.h"
#include "pyint.h"
#include "pybool.h"
#include "pystr.h"
#include "methods.h"
#include "../list.h"
#include "../string.h"
#include "../builtins/builtins.h"

pytuple *pytuple__init__() {
    pytuple *retptr = (pytuple *)malloc(sizeof(pytuple));
    retptr->type = pytuple_t;
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

    return retptr;
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

pybool *pytuple__gt__(void *lvoid, void *rvoid) {
    pytuple *left = lvoid;
    if (type(rvoid) == pytuple_t) {
        pytuple *right = rvoid;
        if (list_is_empty(left->values) || list_is_empty(right->values))
            return PYBOOL(!list_is_empty(left->values));
        list *ptr1, *ptr2;
        for (ptr1 = left->values, ptr2 = right->values; ptr1 && ptr2;
                  ptr1 = ptr1->next, ptr2 = ptr2->next) {
            list *value_list = list_node();
            list_append_content(value_list, ptr1->content);
            list_append_content(value_list, ptr2->content);
            if (is_true(__gt__(pyargument_init2(value_list))))
                return PYBOOL(1);
            else if (is_true(__lt__(pyargument_init2(value_list))))
                return PYBOOL(0);
        }
        if (ptr1)
            return PYBOOL(1);
        return PYBOOL(0);
    }
}

pybool *pytuple__lt__(void *lvoid, void *rvoid) {
    pytuple *left = lvoid;
    if (type(rvoid) == pytuple_t) {
        pytuple *right = rvoid;
        if (list_is_empty(left->values) || list_is_empty(right->values))
            return PYBOOL(!list_is_empty(right->values));
        list *ptr1, *ptr2;
        for (ptr1 = left->values, ptr2 = right->values; ptr1 && ptr2;
                  ptr1 = ptr1->next, ptr2 = ptr2->next) {
            list *value_list = list_node();
            list_append_content(value_list, ptr1->content);
            list_append_content(value_list, ptr2->content);
            if (is_true(__lt__(pyargument_init2(value_list))))
                return PYBOOL(1);
            else if (is_true(__gt__(pyargument_init2(value_list))))
                return PYBOOL(0);
        }
        if (ptr2)
            return PYBOOL(1);
        return PYBOOL(0);
    }
}

pybool *pytuple__eq__(void *lvoid, void *rvoid) {
    pytuple *left = lvoid;
    if (type(rvoid) == pytuple_t) {
        pytuple *right = rvoid;
        if (list_is_empty(left->values) || list_is_empty(right->values))
            return PYBOOL(list_is_empty(left->values) && list_is_empty(right->values));
        list *ptr1, *ptr2;
        for (ptr1 = left->values, ptr2 = right->values; ptr1 && ptr2;
                  ptr1 = ptr1->next, ptr2 = ptr2->next) {
            list *value_list = list_node();
            list_append_content(value_list, ptr1->content);
            list_append_content(value_list, ptr2->content);
            if (!is_true(__eq__(pyargument_init2(value_list))))
                return PYBOOL(0);
        }
        if (ptr1 || ptr2)
            return PYBOOL(0);
        return PYBOOL(1);
    }
    return PYBOOL(0);
}
