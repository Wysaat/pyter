#include <stdio.h>
#include "../pytype/pyint.h"
#include "../pytype/pybool.h"
#include "../pytype/pyfloat.h"
#include "../pytype/pytuple.h"
#include "../pytype/pycomplex.h"
#include "../pytype/pyfunction.h"
#include "../pytype/pyclass.h"
#include "../pytype/pygenerator.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../pytype/py__builtins__.h"
#include "../types.h"
#include "../evaluate.h"
#include "builtins.h"

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


void *__mod__(pyargument *argument) {
    void *left = argument->value_list->content;
    void *right = argument->value_list->next->content;
    if (type(left) == pyint_t)
        return pyint__mod__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__mod__(left, right);
    else if (type(left) == instance_t) {
        void *func = env_find(((instance *)left)->class->env, "__mod__");
        return __call__(func, argument);
    }
}

void *__gt__(pyargument *argument) {
    void *left = argument->value_list->content;
    void *right = argument->value_list->next->content;
    if (type(left) == pyint_t)
        return pyint__gt__(left, right);
    else if (type(left) == pybool_t)
        return pybool__gt__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__gt__(left, right);
    else if (type(left) == pystr_t)
        return pystr__gt__(left, right);
    else if (type(left) == pylist_t)
        return pylist__gt__(left, right);
    else if (type(left) == pytuple_t)
        return pytuple__gt__(left, right);
    // else if (type(left) == instance_t) {
    //     void *func = env_find(((instance *)left)->class->env, "__gt__");
    //     return __call__(func, argument);
    // }
    else if (type(left) == instance_t) {
        void *func;
        pyclass *class = ((instance *)left)->class;
        if (func = pyclass__getattribute__(class, left, "__gt__")) {
            if (type(func) == pyfunction_t)
                ((pyfunction *)func)->bound = 0;
            else if (type(func) == pybuiltin_function_t)
                ((pybuiltin_function *)func)->bound = 0;
            return __call__(func, argument);
        }
    }
}

void *__lt__(pyargument *argument) {
    void *left = argument->value_list->content;
    void *right = argument->value_list->next->content;
    if (type(left) == pyint_t)
        return pyint__lt__(left, right);
    else if (type(left) == pybool_t)
        return pybool__lt__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__lt__(left, right);
    else if (type(left) == pystr_t)
        return pystr__lt__(left, right);
    else if (type(left) == pylist_t)
        return pylist__lt__(left, right);
    else if (type(left) == pytuple_t)
        return pytuple__lt__(left, right);
    else if (type(left) == instance_t) {
        void *func = env_find(((instance *)left)->class->env, "__lt__");
        return __call__(func, argument);
    }
}

void *__eq__(pyargument *argument) {
    void *left = argument->value_list->content;
    void *right = argument->value_list->next->content;
    if (type(left) == instance_t) {
        void *func;
        if (func = env_find(((instance *)left)->class->env, "__eq__"))
            return __call__(func, argument);
    }
    if (type(right) == instance_t) {
        void *func;
        if (func = env_find(((instance *)right)->class->env, "__eq__"))
            return __call__(func, argument);
    }
    if (type(left) == instance_t)
        return PYBOOL(0);
    if (type(left) == pyint_t)
        return pyint__eq__(left, right);
    else if (type(left) == pybool_t)
        return pybool__eq__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__eq__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__eq__(left, right);
    else if (type(left) == pystr_t)
        return pystr__eq__(left, right);
    else if (type(left) == pylist_t)
        return pylist__eq__(left, right);
    else if (type(left) == pytuple_t)
        return pytuple__eq__(left, right);
}
