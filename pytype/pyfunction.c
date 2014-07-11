#include <stdlib.h>
#include "pyfunction.h"
#include "../struct_info.h"
#include "../environment.h"
#include "pygenerator.h"
#include "../list.h"
#include "../execute.h"
#include "others.h"
#include "../gen_execute.h"
#include "../types.h"
#include "../builtins/builtins.h"

pyfunction *pyfunction_init() {
    pyfunction *retptr = (pyfunction *)malloc(sizeof(pyfunction));
    retptr->type = pyfunction_t;
    retptr->class = &function_class;
    return retptr;
}

void *pyfunction__call__(void *lptr, void *rptr) {
    pyfunction *func = (pyfunction *)lptr;
    list *ptr1, *ptr2;
    environment *local_env = environment_init(func->env);
    if (func->assign_target_list)
        store(local_env, func->assign_target_list, func->assign_values);

    if (func->parameters) {
        ptr1 = func->parameters;
        if (func->bound) {
            store(local_env, func->parameters->content, func->bound);
            ptr1 = func->parameters->next;
        }

        if (rptr) {
            pyargument *argument = (pyargument *)rptr;
            if (ptr1 && ptr1->content && argument->value_list) {
                for (ptr2 = argument->value_list; ptr2; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                    store(local_env, ptr1->content, ptr2->content);
                }
            }

            if (argument->assign_target_list) {
                for (ptr1 = argument->assign_target_list, ptr2 = argument->assign_value_list;
                         ptr1 && ptr2; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                    store(local_env, ptr1->content, ptr2->content);
                }
            }
        }
    }

    if (func->yield)
        return pygenerator_init(copy(func->fsuite), local_env, func->id->value);

    execute(func->fsuite, local_env, 0);

    void *retptr;
    if (local_env->ret)
        retptr = local_env->ret;
    else
        retptr = pyNone_init();

    return retptr;
}
