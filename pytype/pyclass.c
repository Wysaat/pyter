#include "pyclass.h"
#include <string.h>
#include <stdlib.h>
#include "../types.h"
#include "pyfunction.h"
#include "../environment.h"
#include "py__builtins__.h"
#include "../__builtins__.h"
#include "pylist.h"
#include "methods.h"

pyclass *pyclass__init__(char *id) {
    pyclass *retptr = (pyclass *)malloc(sizeof(pyclass));
    retptr->type = pyclass_t;
    retptr->class = (pyclass *)malloc(sizeof(pyclass));
    retptr->class->type = pyclass_t;
    retptr->class->id = strdup("type");
    retptr->id = id;
    retptr->env = environment_init(0);
    return retptr;
}

void *pyclass__getattribute__(void *first, void *instance, pystr *attr) {
    pyclass *class = (pyclass *)first;
    list *ptr;
    if (!list_is_empty(class->env->val_dict)) {
        for (ptr = class->env->val_dict; ptr; ptr = ptr->next) {
            val_dict_entry *entry = (val_dict_entry *)ptr->content;
            if (!strcmp(entry->id, attr->value)) {
                if (type(entry->value) == pyfunction_t) {
                    ((pyfunction *)entry->value)->bound = instance;
                }
                else if (type(entry->value) == pybuiltin_function_t) {
                    ((pybuiltin_function *)entry->value)->bound = instance;
                }
                return entry->value;
            }
        }
    }
}

void *pyclass__call__(void *left, void *right) {
    if (left == &list_class)
        return pylist__init__();
    instance *retptr = (instance *)malloc(sizeof(instance));
    retptr->type = instance_t;
    retptr->class = (pyclass *)left;
    retptr->env = environment_init(0);

    list *ptr;
    for (ptr = retptr->class->env->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (entry->id && !strcmp(entry->id, "__init__")) {
            ((pyfunction *)entry->value)->bound = retptr;
            __call__(entry->value, right);
        }
    }

    return retptr;
}

void pyclass__setattr__(void *first, void *second, pystr *attr, void *val) {
    if (type(second) == instance_t) {
        instance *inst = (instance *)second;
        store(inst->env, IDENTIFIER(attr->value), val);
    }
    else if (type(second) == pyclass_t) {
        pyclass *class = (pyclass *)second;
        store(class->env, IDENTIFIER(attr->value), val);
    }
}