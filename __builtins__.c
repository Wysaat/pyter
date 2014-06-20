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
#include "pytype/methods.h"

void *_print(list *val) {
    if (val && !list_is_empty(val)) {
        for ( ; val; val = val->next) {
            if (type(val->content) == pystr_t)
                printf("%s", ((pystr *)val->content)->value);
            else
                print_nnl(val->content);
            printf(" ");
        }
    }
    printf("\n");
    return pyNone_init();
}

void def_print(environment *env) {
    pybuiltin_function *print_func = pybuiltin_function__init__("print", _print);
    store_id(env, "print", print_func);
}

void *next(list *val) {
    if (type(val->content) == pygenerator_t)
        return pygenerator_next(val->content);
}

void def_next(environment *env) {
    pybuiltin_function *next_func = pybuiltin_function__init__("next", next);
    store_id(env, "next", next_func);
}

void *_len(list *val) {
    return len(val->content);
}

void def_len(environment *env) {
    pybuiltin_function *len_func = pybuiltin_function__init__("len", _len);
    store_id(env, "len", len_func);
}

void def_int(environment *env) {
    int_class.type = pyclass_t;
    int_class.ref = 0;
    int_class.class = &type_class;
    int_class.id = "int";
    int_class.env = environment_init(0);
    int_class.inheritance = 0;
    store_id(env, "int", &int_class);
}

void def_str(environment *env) {
    str_class.type = pyclass_t;
    str_class.ref = 0;
    str_class.class = &type_class;
    str_class.id = "str";
    str_class.env = environment_init(0);
    str_class.inheritance = 0;
    store_id(env, "str", &str_class);
}

/* CAUTION: nested function definition is not standard C, change it someday.. */
void *def_sort_func_of_list(environment *env) {
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
    store_id(env, "sort", sort_func_of_list);
}

void *_pylist_append(list *val) {
    pylist__append__(val->content, val->next->content);
    return pyNone_init();
}

void def_append_func_of_list(environment *env) {
    pybuiltin_function *append_func_of_list = pybuiltin_function__init__("append", _pylist_append);
    store_id(env, "append", append_func_of_list);
}

void def_list(environment *env) {
    list_class.type = pyclass_t;
    list_class.ref = 0;
    list_class.class = &type_class;
    list_class.id = "list";
    list_class.env = environment_init(0);
    list_class.inheritance = 0;
    def_sort_func_of_list(list_class.env);
    def_append_func_of_list(list_class.env);
    store_id(env, "list", &list_class);
}

void def_range(environment *env) {
    range_class.type = pyclass_t;
    range_class.ref = 0;
    range_class.class = &type_class;
    range_class.id = "range";
    range_class.env = environment_init(0);
    range_class.inheritance = 0;
    store_id(env, "range", &range_class);
}

void def_type(environment *env) {
    type_class.type = pyclass_t;
    type_class.ref = 0;
    type_class.class = &type_class;
    type_class.id = "type";
    type_class.env = environment_init(0);
    type_class.inheritance = 0;
    store_id(env, "type", &type_class);
}

void def__builtins__(environment *env) {
    __builtins__module.type = pymodule_t;
    __builtins__module.ref = 0;
    __builtins__module.name = "__builtins__";
    __builtins__module.env = environment_init(0);
    store_id(env, "__builtins__", &__builtins__module);
    def_print(__builtins__module.env);
    def_next(__builtins__module.env);
    def_len(__builtins__module.env);
    def_int(__builtins__module.env);
    def_str(__builtins__module.env);
    def_list(__builtins__module.env);
    def_range(__builtins__module.env);
    def_type(__builtins__module.env);
}