#include <stdio.h>
#include "../pytype/pyint.h"
#include "../pytype/pybool.h"
#include "../pytype/pyfloat.h"
#include "../pytype/pycomplex.h"
#include "../pytype/pyclass.h"
#include "../pytype/pygenerator.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../types.h"
#include "../evaluate.h"

void *_abs(pyargument *argument) {
    void *vptr = argument->value_list->content;
    if (type(vptr) == pyint_t)
        return pyint__abs__(vptr);
    // else if (type(vptr) == pybool_t)
    //     return pybool__abs__(vptr);
    else if (type(vptr) == pyfloat_t)
        return pyfloat__abs__(vptr);
    else if (type(vptr) == pycomplex_t)
        return pycomplex__abs__(vptr);
    else if (type(vptr) == instance_t)
        return __call__(env_find(((instance *)vptr)->class->env, "__abs__"), argument);
}

void *_len(pyargument *argument) {
    list *val = argument->value_list;
    return len(val->content);
}

void *_next(pyargument *argument) {
    list *val = argument->value_list;
    if (type(val->content) == pygenerator_t)
        return pygenerator_next(val->content);
}

void *_print(pyargument *argument) {
    char *sep = " ";
    char *end = "\n";
    FILE *file = stdout;
    int flush = 0;
    if (!argument) {
        printf("%s", end);
        return pyNone_init();
    }
    list *val = argument->value_list;
    if (argument->assign_target_list) {
        list *ptr, *ptr2;
        for (ptr = argument->assign_target_list, ptr2 = argument->assign_value_list;
                    ptr && ptr2; ptr = ptr->next, ptr2 = ptr2->next) {
            identifier *id = (identifier *)ptr->content;
            if (!strcmp(id->value, "sep")) {
                if (type(ptr2->content) == pystr_t)
                    sep = ((pystr *)ptr2->content)->value;
            }
            else if (!strcmp(id->value, "end")) {
                if (type(ptr2->content) == pystr_t)
                    end = ((pystr *)ptr2->content)->value;
            }
        }
    }
    if (val && !list_is_empty(val)) {
        for ( ; val->next; val = val->next) {
            if (type(val->content) == pystr_t)
                printf("%s", ((pystr *)val->content)->value);
            else
                print_nnl(val->content);
            printf("%s", sep);
        }
        if (type(val->content) == pystr_t)
            printf("%s", ((pystr *)val->content)->value);
        else
            print_nnl(val->content);
    }
    printf("%s", end);
    if (flush)
        fflush(file);
    return pyNone_init();
}


void __mod__(pyargument *argument) {}