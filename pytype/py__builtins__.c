#include "py__builtins__.h"
#include "../types.h"
#include <stdlib.h>

pybuiltin_function *pybuiltin_function__init__(char *id, void *func) {
    pybuiltin_function *retptr = (pybuiltin_function *)malloc(sizeof(pybuiltin_function));
    retptr->type = pybuiltin_function_t;
    retptr->id = id;
    retptr->func = func;
    return retptr;
}

void *pybuiltin_function__call__(void *left, void *right) {
    pybuiltin_function *builtin_func = (pybuiltin_function *)left;
    return builtin_func->func(right);
}