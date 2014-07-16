#include <stdlib.h>
#include "others.h"
#include "../types.h"
#include "../struct_info.h"
#include "../environment.h"
#include "../builtins/builtins.h"
#include "../list.h"
#include "pyint.h"
#include "../integer.h"
#include "../string.h"
#include "pystr.h"

pyNone *pyNone_init() {
    pyNone *retptr = (pyNone *)malloc(sizeof(pyNone));
    retptr->type = pyNone_t;
    retptr->class = &NoneType_class;
    return retptr;
}

pyrange *pyrange_init(void *vptr) {
    pyrange *retptr = (pyrange *)malloc(sizeof(pyrange));
    retptr->type = pyrange_t;
    retptr->class = &range_class;
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
    return retptr;
}

pyrange *pyrange_init2(pyint *start, pyint *stop, pyint *step) {
    pyrange *retptr = (pyrange *)malloc(sizeof(pyrange));
    retptr->type = pyrange_t;
    retptr->class = &range_class;
    retptr->start = start;
    retptr->stop = stop;
    retptr->step = step;
    return retptr;
}

pyrange *pyrange_init3(int start, int stop, int step) {
    pyrange *retptr = (pyrange *)malloc(sizeof(pyrange));
    retptr->type = pyrange_t;
    retptr->class = &range_class;
    retptr->start = int_to_pyint(start);
    retptr->stop = int_to_pyint(stop);
    retptr->step = int_to_pyint(step);
    return retptr;
}

void *pyrange__getitem__(void *lvoid, void *rvoid) {
    pyrange *range = (pyrange *)lvoid;
    if (type(rvoid) == pyint_t) {
        int right = pyint_to_int(rvoid);
        int start = pyint_to_int(range->start);
        int step = pyint_to_int(range->step);
        if (((pyint *)rvoid)->value->sign == '-')
            right += pyrange_len2(range);
        return int_to_pyint(start+step*right);
    }
    else if (type(rvoid) == pyslice_t) {
        pyslice *slice = (pyslice *)rvoid;
        int length = pyrange_len2(range);
        int start, stop, step = slice->step;
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

        pyint *_start = pyrange_getitem2(range, start);
        pyint *_stop = pyrange_getitem2(range, stop);
        pyint *_step = int_to_pyint(step * pyint_to_int(range->step));
        return pyrange_init2(_start, _stop, _step);
    }
}

void *pyrange_getitem2(void *lvoid, int pos) {
    pyrange *range = (pyrange *)lvoid;
    int start = pyint_to_int(range->start);
    int step = pyint_to_int(range->step);
    if (pos < 0)
        pos += pyrange_len2(range);
    return int_to_pyint(start+step*pos);
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

pyint *pyrange_len(void *vptr) {
    pyrange *range = (pyrange *)vptr;
    int start = pyint_to_int(range->start);
    int stop = pyint_to_int(range->stop);
    int step = pyint_to_int(range->step);

    if (start > stop && step > 0)
        return int_to_pyint(0);
    else if (stop > start && step < 0)
        return int_to_pyint(0);

    if (step < 0) {
        int tmp = start;
        start = stop;
        stop = tmp;
        step = -step;
    }
    int len = (stop-start-1) / step + 1;
    return int_to_pyint(len);
}

int pyrange_len2(void *vptr) {
    pyrange *range = (pyrange *)vptr;
    int start = pyint_to_int(range->start);
    int stop = pyint_to_int(range->stop);
    int step = pyint_to_int(range->step);

    if (start > stop && step > 0)
        return 0;
    else if (stop > start && step < 0)
        return 0;

    if (step < 0) {
        int tmp = start;
        start = stop;
        stop = tmp;
        step = -step;
    }
    return (stop-start-1) / step + 1;
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

pyargument *pyargument_init() {
    pyargument *retptr = (pyargument *)malloc(sizeof(pyargument));
    retptr->type = __pyargument_t;
    return retptr;
}

pyargument *pyargument_init2(list *value_list) {
    pyargument *retptr = (pyargument *)malloc(sizeof(pyargument));
    retptr->type = __pyargument_t;
    retptr->assign_target_list = 0;
    retptr->assign_value_list = 0;
    retptr->value_list = value_list;
    return retptr;
}

pyargument *pyargument_init3(void *left, void *right) {
    pyargument *retptr = (pyargument *)malloc(sizeof(pyargument));
    retptr->type = __pyargument_t;
    retptr->assign_target_list = 0;
    retptr->assign_value_list = 0;
    retptr->value_list = list_node();
    list_append_content(retptr->value_list, left);
    list_append_content(retptr->value_list, right);
    return retptr;
}