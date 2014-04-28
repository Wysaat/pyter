#include "pyclass.h"
#include <string.h>
#include <stdlib.h>
#include "../types.h"
#include "pyfunction.h"

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
                return entry->value;
            }
        }
    }
}

void *pyclass__call__(void *left, void *right) {
    instance *retptr = (instance *)malloc(sizeof(instance));
    retptr->type = instance_t;
    retptr->class = (pyclass *)left;
    retptr->env = environment_init(0);
    return retptr;
}