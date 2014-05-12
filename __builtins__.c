#include "pytype/py__builtins__.h"
#include "evaluate.h"
#include "environment.h"
#include <stdio.h>
#include "types.h"
#include "pytype/pytuple.h"
#include "pytype/pygenerator.h"

void *_print(void *val) {
    if (val) {
        if (type(val) == pytuple_t) {
            list *ptr;
            for (ptr = ((pytuple *)val)->values; ptr; ptr = ptr->next) {
                print_nnl(ptr->content);
                printf(" ");
            }
        }
        else
            print_nnl(val);
    }
    printf("\n");
    return 0;
}

void *next(void *val) {
    if (type(val) == pygenerator_t)
        return pygenerator_next(val);
}

void def_print(environment *env) {
    pybuiltin_function *print_func = pybuiltin_function__init__("print", _print);
    store(env, IDENTIFIER("print"), print_func);
}

void def_next(environment *env) {
    pybuiltin_function *next_func = pybuiltin_function__init__("next", next);
    store(env, IDENTIFIER("next"), next_func);
}