#include "pytype/py__builtins__.h"
#include "evaluate.h"
#include "environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "pytype/pytuple.h"
#include "pytype/pygenerator.h"
#include "pytype/pylist.h"
#include "pytype/pyint.h"
#include "pytype/pybool.h"
#include "pytype/pyclass.h"
#include "__builtins__.h"
#include "pytype/others.h"

void *_print(list *val) {
    if (val && !list_is_empty(val)) {
        for ( ; val; val = val->next) {
            print_nnl(val->content);
            printf(" ");
        }
    }
    printf("\n");
    return pyNone_init();
}

void *next(list *val) {
    if (type(val->content) == pygenerator_t)
        return pygenerator_next(val->content);
}

void def_print(environment *env) {
    pybuiltin_function *print_func = pybuiltin_function__init__("print", _print);
    store(env, IDENTIFIER("print"), print_func);
}

void def_next(environment *env) {
    pybuiltin_function *next_func = pybuiltin_function__init__("next", next);
    store(env, IDENTIFIER("next"), next_func);
}

void def_int(environment *env) {
    store(env, IDENTIFIER("int"), &int_class);
}

void def_sort_func_of_list(environment *env) {
    int comp(void *left, void *right) {
        if (type(left) == pyint_t && type(right) == pyint_t) {
            if (is_true(pyint__lt__((pyint *)left, (pyint *)right)))
                return -1;
            else if (is_true(pyint__eq__((pyint *)left, (pyint *)right)))
                return 0;
            else
                return 1;
        }
    }
    void *sort_of_list(list *val) {
        pylist__sort__((pylist *)val->content, comp);
        return pyNone_init();
    }
    pybuiltin_function *sort_func_of_list = pybuiltin_function__init__("sort", sort_of_list);
    store(env, IDENTIFIER("sort"), sort_func_of_list);
}

// void def_len_func_of_list(environment *env) {
//     void
// }

void def_list(environment *env) {
    list_class.type = pyclass_t;
    list_class.class = (pyclass *)malloc(sizeof(pyclass));
    list_class.class->type = pyclass_t;
    list_class.class->id = strdup("type");
    list_class.id = "list";
    list_class.env = environment_init(0);
    def_sort_func_of_list(list_class.env);
    store(env, IDENTIFIER("list"), &list_class);
}