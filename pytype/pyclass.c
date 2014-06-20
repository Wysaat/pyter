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
#include <string.h>
#include "../builtins/builtins.h"

pyclass *pyclass__init__(char *id) {
    pyclass *retptr = (pyclass *)malloc(sizeof(pyclass));
    memset(retptr, 0, sizeof(pyclass));
    retptr->type = pyclass_t;
    retptr->class = &type_class;
    retptr->id = id;
    retptr->env = environment_init(0);
    retptr->ref = 0;
    retptr->inheritance = 0;
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
    ref(left);
    if (left == &int_class) {
    }
    else if (left == &float_class) {
    }
    else if (left == &complex_class) {
    }
    else if (left == &str_class) {
        void *retptr = str_init(right);
        ref(retptr);
        return retptr;
    }
    else if (left == &list_class) {
        void *retptr = pylist__init__();
        ref(retptr);
        return retptr;
    }
    else if  (left == &range_class) {
        void *retptr = pyrange_init(right);
        ref(retptr);
        return retptr;
    }
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

    ref(retptr);
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

void pyclass_del(void *vptr) {
    ref_dec(vptr);
    pyclass *class = (pyclass *)vptr;
    if (class->inheritance) {
        list *ptr;
        for (ptr = class->inheritance; ptr; ptr = ptr->next)
            del(ptr->content);
    }
    if (get_ref(vptr) == 0) {
        free(class->id);
        del(class->env);
        if (class->inheritance) {
            list *ptr, *tmp;
            while (ptr) {
                tmp = ptr;
                ptr = ptr->next;
                free(tmp);
            }
        }
        free(class);
    }
}

void pyclass_ref(void *vptr) {
    ref_inc(vptr);
    pyclass *class = (pyclass *)vptr;
    if (class->inheritance) {
        list *ptr;
        for (ptr = class->inheritance; ptr; ptr = ptr->next)
            ref(ptr->content);
    }
}