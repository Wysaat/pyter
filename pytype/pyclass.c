#include <stdlib.h>
#include <string.h>
#include "pyclass.h"
#include "../types.h"
#include "../environment.h"
#include "../list.h"
#include "../struct_info.h"
#include "pyfunction.h"
#include "py__builtins__.h"
#include "../builtins/builtins.h"
#include "pylist.h"
#include "others.h"
#include "methods.h"

pyclass *pyclass__init__(char *id) {
    pyclass *retptr = (pyclass *)malloc(sizeof(pyclass));
    memset(retptr, 0, sizeof(pyclass));
    retptr->type = pyclass_t;
    retptr->class = &type_class;
    retptr->id = id;
    retptr->env = environment_init(0);
    retptr->inheritance = 0;
    return retptr;
}

void *pyclass__getattribute__(void *first, void *instance, char *attr) {
    pyclass *class = (pyclass *)first;
    void *retptr;
    list *ptr, *ptr2, *ptr3;
    if (retptr = env_find(class->env, attr)) {
        if (type(retptr) == pyfunction_t)
            ((pyfunction *)retptr)->bound = instance;
        else if (type(retptr) == pybuiltin_function_t)
            ((pybuiltin_function *)retptr)->bound = instance;
        return retptr;
    }
    for (ptr2 = class->inheritance; ptr2; ptr2 = ptr2->next) {
        pyclass *_class = ptr2->content;
        if (retptr = pyclass__getattribute__(_class, instance, attr))
            return retptr;
    }
    return 0;
}

void *pyclass__call__(void *left, void *right) {
    if (left == &int_class) {
        return int_init(right);
    }
    else if (left == &bool_class) {
        return bool_init(right);
    }
    else if (left == &float_class) {
        return float_init(right);
    }
    else if (left == &complex_class) {
    }
    else if (left == &str_class) {
        void *retptr = str_init(right);
        return retptr;
    }
    else if (left == &list_class) {
        return list_init(right);
    }
    else if  (left == &range_class) {
        void *retptr = pyrange_init(right);
        return retptr;
    }

    instance *retptr = instance_init((pyclass *)left);
    list *ptr;
    // CAUTION! val_dict can be empty
    if (list_is_empty(retptr->class->env->val_dict))
        return retptr;
    for (ptr = retptr->class->env->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (entry->id && !strcmp(entry->id, "__init__")) {
            ((pyfunction *)entry->value)->bound = retptr;
            __call__(entry->value, right);  // __init__ should return None
        }
    }
    return retptr;
}

void pyclass__setattr__(void *first, void *second, char *attr, void *val) {
    if (type(second) == instance_t) {
        instance *inst = (instance *)second;
        store(inst->env, IDENTIFIER(attr), val);
    }
    else if (type(second) == pyclass_t) {
        pyclass *class = (pyclass *)second;
        store(class->env, IDENTIFIER(attr), val);
    }
}

instance *instance_init(pyclass *class) {
    instance *retptr = (instance *)malloc(sizeof(instance));
    retptr->type = instance_t;
    retptr->class = class;
    retptr->env = environment_init(0);
    return retptr;
}
