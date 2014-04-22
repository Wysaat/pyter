#include "../cpyter.h"
#include "pybool.h"
#include "../list.h"
#include "others.h"

pylist *pylist__init__() {
    pylist *retptr = (pylist *)malloc(sizeof(pylist));
    retptr->type = pylist_t;
    return retptr;
}

void *pylist__sort__(pylist *val, int comp()) {
    list_sort(val->values, comp);
}

pylist *pylist__add__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pylist *right = (pylist *)rvoid;
    pylist *retptr = pylist__init__();
    retptr->values = list_add(left->values, right->values);
    return retptr;
}

void pylist__append__(pylist *left, void *rvoid) {
    list_append_content(left->values, rvoid);
}

pylist *pylist__mul__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pyint *times = (pyint *)rvoid;
    pylist *retptr = pylist__init__();
    retptr->values = list_node();

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

pybool *pylist__eq__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    pylist *right = (pylist *)rvoid;
    return PYBOOL(list_eq(left->values, right->values));
}

void *pylist__getitem__(void *lvoid, void *rvoid) {
    pylist *left = (pylist *)lvoid;
    list *ptr;
    pyint *ind = pyint__init__();
    ind->value = INTEGER_NODE();
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        for (ptr = left->values; is_true(pyint__lt__(ind, right)); pyint__inc__(ind))
            ptr = ptr->next;
        pyint__del__(ind);
        return ptr->content;
    }
    else if (type(rvoid) == (pyslice_t)) {
        pylist *retptr = pylist__init__();
        retptr->values = list_node();
        pyint *zero = pyint__init__(); zero->value = INTEGER_NODE();
        pyint *delt, *mo;
        pyslice *right = (pyslice *)rvoid;
        for (ptr = left->values; is_true(pyint__lt__(ind, right->stop)); 
                  pyint__inc__(ind), ptr = ptr->next) {
            if (is_true(pyint__ge__(ind, right->start))) {
                delt = pyint__sub__(ind, right->start);
                mo = pyint__mod__(delt, right->step);
                if (is_true(pyint__eq__(mo, zero))) {
                    pylist__append__(retptr, ptr->content);
                }
                pyint__del__(mo);
                pyint__del__(delt);
            }
        }
        pyint__del__(zero);
        pyint__del__(ind);
        return retptr;
    }
}

void pylist__setitem__(void *lvoid, void *rvoid, void *value) {
    pylist *left = (pylist *)lvoid;
    list *ptr;
    pyint *ind = pyint__init__();
    ind->value = INTEGER_NODE();
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        for (ptr = left->values; is_true(pyint__lt__(ind, right)); pyint__inc__(ind))
            ptr = ptr->next;
        pyint__del__(ind);
        ptr->content = value;
    }
    else if (type(rvoid) == pyslice_t) {
        pyslice *right = (pyslice *)rvoid;
        pyint *one = pyint__init__();
        one->value = INTEGER_NODE();
        one->value->value = 1;
        list *ptr2;
        if (is_true(pyint__eq__(right->step, one))) {
            for (ptr = left->values; is_true(pyint__lt__(ind, right->start)); pyint__inc__(ind))
                ptr = ptr->next;
            ptr = ptr->prev;
            ind->value->value = 0;
            for (ptr2 = left->values; is_true(pyint__lt__(ind, right->stop)); pyint__inc__(ind))
                ptr2 = ptr2->next;
            if (type(value) == pylist_t) {
                list *to_add = list_cpy(((pylist *)value)->values);
                ptr->next = to_add;
                to_add->prev = ptr;
                list *ptr3 = to_add;
                while (ptr3->next)
                    ptr3 = ptr3->next;
                ptr3->next = ptr2;
                ptr2->prev = ptr3;
                pyint__del__(one);
                pyint__del__(ind);
            }
        }
        else {
            pyint *zero = pyint__init__(); zero->value = INTEGER_NODE();
            pyint *delt, *mo;
            if (type(value) == pylist_t) {
                pylist *to_add = (pylist *)value;
                for (ptr = left->values, ptr2 = to_add->values; is_true(pyint__lt__(ind, right->stop)); 
                          pyint__inc__(ind), ptr = ptr->next) {
                    if (is_true(pyint__ge__(ind, right->start))) {
                        delt = pyint__sub__(ind, right->start);
                        mo = pyint__mod__(delt, right->step);
                        if (is_true(pyint__eq__(mo, zero))) {
                            ptr->content = ptr2->content;
                            ptr2 = ptr2->next;
                        }
                        pyint__del__(mo);
                        pyint__del__(delt);
                    }
                }
                pyint__del__(zero);
                pyint__del__(ind);
            }
        }
    }
}