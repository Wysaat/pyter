#include <stdlib.h>
#include <string.h>
#include "pylist.h"
#include "../types.h"
#include "../list.h"
#include "../string.h"
#include "../struct_info.h"
#include "../environment.h"
#include "pyint.h"
#include "pybool.h"
#include "others.h"
#include "pytuple.h"
#include "pyset.h"
#include "methods.h"
#include "../builtins/builtins.h"

pylist *pylist__init__() {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    retptr->class = &list_class;
    retptr->values = list_node();
    return retptr;
}

pylist *pylist_init2(list *values) {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    retptr->class = &list_class;
    retptr->values = values;
    return retptr;
}

void pylist__sort__(pylist *val, int comp()) {
    list_sort(val->values, comp);
}

pylist *pylist__add__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pylist *right = (pylist *)rvoid;
    if (list_is_empty(left->values))
        return right;
    if (list_is_empty(right->values))
        return left;
    pylist *retptr = pylist__init__();
    list *ptr = retptr->values;
    list *ptr2 = left->values;
    list *ptr3 = right->values;

    for ( ; ptr2; ptr2 = ptr2->next) {
        ptr->content = ptr2->content;
        ptr->next = list_node();
        ptr->next->prev = ptr;
        ptr = ptr->next;
    }
    for ( ; ptr3; ptr3 = ptr3->next) {
        ptr->content = ptr3->content;
        ptr->next = list_node();
        ptr->next->prev = ptr;
        ptr = ptr->next;
    }
    ptr = ptr->prev;
    free(ptr->next);
    ptr->next = 0;

    return retptr;
}

void pylist__append__(pylist *left, void *rvoid) {
    list_append_content(left->values, rvoid);
}

pylist *pylist__mul__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    if (type(rvoid) == pyint_t) {
        int times = pyint_to_int(rvoid), i;
        pylist *retptr = pylist__init__();
        for (i = 0; i < times; i++) {
            list *ptr;
            for (ptr = left->values; ptr; ptr = ptr->next) {
                list_append_content(retptr->values, ptr->content);
            }
        }
        return retptr;
    }
}

pybool *pylist__eq__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pylist *right = (pylist *)rvoid;
    return PYBOOL(list_eq(left->values, right->values));
}

void *pylist__getitem__(void *lvoid, void *rvoid) {
    pylist *primary = (pylist *)lvoid;
    if (type(rvoid) == pyint_t) {
        int index = pyint_to_int(rvoid);
        if (index < 0)
            index += pyint_to_int(pylist__len__(lvoid));
        int i = 0;
        list *ptr = primary->values;
        while (i++ < index)
            ptr = ptr->next;
        return ptr->content;
    }
    else if (type(rvoid) == pyslice_t) {
        pyslice *slice = (pyslice *)rvoid;
        pylist *retptr = pylist__init__();
        int length = pyint_to_int(pylist__len__(lvoid));
        int start, stop, step;
        step = slice->step;
        if (slice->nostart) {
            if (step > 0)
                start = 0;
            else
                start = length - 1;
        }
        else {
            start = slice->start;
            if (start < 0)
                start += length;
            else if (step < 0 && start >= length)
                start = length - 1;
        }
        if (slice->nostop) {
            if (step > 0)
                stop = length;
            else
                stop = -1;
        }
        else {
            stop = slice->stop;
            if (stop < 0)
                stop += length;
            else if (step > 0 && stop > length)
                stop = length;
        }
        if (step > 0 && start >= stop)
            return pylist__init__();
        if (step < 0 && start <= stop)
            return pylist__init__();

        int i;
        list *ptr;
        for (i = 0, ptr = primary->values; i < start; i++, ptr = ptr->next)
            ;
        if (step > 0) {
            for ( ; i < stop; i++, ptr = ptr->next) {
                if ((i - start) % step == 0)
                    pylist__append__(retptr, ptr->content);
            }
        }
        else {
            for ( ; i > stop; i--, ptr = ptr->prev) {
                if ((i - start) % step == 0)
                    pylist__append__(retptr, ptr->content);
            }
        }
        return retptr;
    }
}

void pylist__setitem__(void *lvoid, void *rvoid, void *value) {
    pylist *primary = (pylist *)lvoid;
    if (type(rvoid) == pyint_t) {
        int index = pyint_to_int(rvoid);
        if (index < 0)
            index += pylist_len2(lvoid);
        int i = 0;
        list *ptr = primary->values;
        while (i++ < index)
            ptr = ptr->next;
        ptr->content = value;
    }
    else if (type(rvoid) == pyslice_t) {
        pyslice *slice = (pyslice *)rvoid;
        int length = pylist_len2(lvoid);
        int start, stop, step;
        step = slice->step;
        if (slice->nostart) {
            if (step > 0)
                start = 0;
            else
                start = length - 1;
        }
        else {
            start = slice->start;
            if (start < 0)
                start += length;
            else if (step < 0 && start >= length)
                start = length - 1;
        }
        if (slice->nostop) {
            if (step > 0)
                stop = length;
            else
                stop = -1;
        }
        else {
            stop = slice->stop;
            if (stop < 0)
                stop += length;
            else if (step > 0 && stop > length)
                stop = length;
        }

        int i;
        list *ptr, *ptr2;
        if (type(value) == pylist_t)
            ptr2 = ((pylist *)value)->values;
        else if (type(value) == pytuple_t)
            ptr2 = ((pytuple *)value)->values;
        else if (type(value) == pyset_t)
            ptr2 = ((pyset *)value)->values;
        if (list_is_empty(ptr2))
            ptr2 = 0;
        for (i = 0, ptr = primary->values; i < start; i++, ptr = ptr->next)
            ;
        if (step > 0) {
            for ( ; i < stop; i++, ptr = ptr->next) {
                if ((i - start) % step == 0) {
                    if (!ptr2) {
                        if (ptr->prev)
                            ptr->prev->next = 0;
                        while (ptr) {
                            list *tmp = ptr->next;
                            free(ptr);
                            ptr = tmp;
                        }
                        return;
                    }
                    ptr->content = ptr2->content;
                    if (!ptr->next && ptr2->next)
                        ptr->next = ptr2->next;
                    ptr2 = ptr2->next;
                }
            }
        }
        else {
            if (!ptr2)
                return;
            for ( ; i > stop; i--, ptr = ptr->prev) {
                if ((i - start) % step == 0) {
                    ptr->content = ptr2->content;
                    ptr2 = ptr2->next;
                }
            }
        }
    }
}

pyint *pylist__len__(void *vptr) {
    pylist *ptr = (pylist *)vptr;
    return int_to_pyint(list_len(ptr->values));
}

int pylist_len2(void *vptr) {
    pylist *ptr = (pylist *)vptr;
    return list_len(ptr->values);
}

pystr *pylist_str(void *vptr) {
    pylist *lptr = (pylist *)vptr;
    buffer2 *buff = buff2_init();
    buff2_add(buff, strdup("["));
    list *ptr;
    for (ptr = lptr->values; ptr->next; ptr = ptr->next) {
        buff2_add(buff, str(ptr->content)->value);
        buff2_add(buff, strdup(", "));
    }
    buff2_add(buff, str(ptr->content)->value);
    buff2_add(buff, strdup("]"));
    return pystr_init3(buff2_puts2(buff));
}
