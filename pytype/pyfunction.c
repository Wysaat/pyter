#include "pyfunction.h"
#include "../types.h"
#include "pytuple.h"
#include "../execute.h"

void *pyfunction__call__(void *lptr, void *right) {
    pyfunction *func = (pyfunction *)lptr;
    environment *local_env = environment_init(func->env);
    list *ptr1, *ptr2;
    if (right) {
        if (type(right) == pytuple_t)
            for (ptr1 = func->parameters, ptr2 = ((pytuple *)right)->values;
                 ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                store(local_env, ptr1->content, ptr2->content);
            }
        else {
            store(local_env, func->parameters->content, right);
        }
    }

    execute(func->fsuite, local_env, 0);

    if (local_env->ret)
        return local_env->ret;
    else
        return 0;
}