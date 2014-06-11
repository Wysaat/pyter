#include "../cpyter.h"
#include "pybool.h"
#include "../list.h"
#include "others.h"

pylist *pylist__init__() {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    retptr->values = list_node();
    return retptr;
}

void pylist__sort__(pylist *val, int comp()) {
    list_sort(val->values, comp);
}

pylist *pylist__add__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pylist *right = (pylist *)rvoid;
    pylist *retptr = pylist__init__();
    list_append_list(retptr->values, list_add(left->values, right->values));
    return retptr;
}

void pylist__append__(pylist *left, void *rvoid) {
    list_append_content(left->values, rvoid);
}

/* lvoid, rvoid should not be changed */
pylist *pylist__mul__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pyint *times = pyint_cpy(rvoid);
    pylist *retptr = pylist__init__();

    pyint *zero = int_to_pyint(0);

    while (is_true(pyint__gt__(times, zero))) {
        list *to_append = list_cpy(left->values);
        list_append_list(retptr->values, to_append);
        pyint__dec__(times);
    }
    pyint__del__(zero);
    pyint__del__(times);

    return retptr;  
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
            index += pyint_to_int(pylist__len__(lvoid));
        int i = 0;
        list *ptr = primary->values;
        while (i++ < index)
            ptr = ptr->next;
        ref_dec(ptr->content);
        ptr->content = value;
        ref_inc(ptr->content);
    }
    else if (type(rvoid) == pyslice_t) {
        pyslice *slice = (pyslice *)rvoid;
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
                            ref_dec(ptr->content);
                            free(ptr->content);
                            list *tmp = ptr->next;
                            free(ptr);
                            ptr = tmp;
                        }
                        return;
                    }
                    ref_dec(ptr->content);
                    ptr->content = ptr2->content;
                    ref_inc(ptr->content);
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
                    ref_dec(ptr->content);
                    ptr->content = ptr2->content;
                    ref_inc(ptr->content);
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

void pylist_del(void *vptr) {
    pylist *ptr = (pylist *)vptr;
    del(ptr->values);
    free(ptr);
}