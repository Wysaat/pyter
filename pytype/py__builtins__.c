#include <stdlib.h>
#include <string.h>
#include "py__builtins__.h"
#include "../types.h"
#include "../list.h"
#include "others.h"
#include "../builtins/builtins.h"

pybuiltin_function *pybuiltin_function__init__(char *id, void *func) {
    pybuiltin_function *retptr = (pybuiltin_function *)malloc(sizeof(pybuiltin_function));
    memset(retptr, 0, sizeof(pybuiltin_function));
    retptr->type = pybuiltin_function_t;
    retptr->class = &function_class;
    retptr->id = id;
    retptr->func = func;
    return retptr;
}

void *pybuiltin_function__call__(void *left, void *right) {
    pybuiltin_function *builtin_func = (pybuiltin_function *)left;
    pyargument *argument;
    if (builtin_func->bound) {
        if (right) {
            argument = (pyargument *)right;
        }
        else {
            right = (pyargument *)malloc(sizeof(pyargument));
            argument = right;
        }
        list *new_value_list = list_node();
        list_append_content(new_value_list, builtin_func->bound);
        if (argument->value_list)
            list_append_list(new_value_list, argument->value_list);
        argument->value_list = new_value_list;
    }
    void *retptr = builtin_func->func((pyargument *)right);
    return retptr;
}
