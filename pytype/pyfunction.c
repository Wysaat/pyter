#include "pyfunction.h"
#include "../types.h"
#include "pytuple.h"
#include "../execute.h"
#include "../evaluate.h"
#include "../gen_execute.h"
#include "pygenerator.h"
#include "../list.h"

void *pyfunction__call__(void *lptr, void *right) {
    pyfunction *func = (pyfunction *)lptr;
    list *ptr1, *ptr2;
    environment *local_env = environment_init(func->env);
    if (!list_is_empty(func->assign_targets))
        store(local_env, func->assign_targets, func->assign_values);

    ptr1 = func->parameters;
    if (func->bound) {
        store(local_env, func->parameters->content, func->bound);
        ptr1 = func->parameters->next;
    }
    if (ptr1 && ptr1->content) {
        if (type(right) == pytuple_t)
            for (ptr2 = ((pytuple *)right)->values;
                 ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                store(local_env, ptr1->content, ptr2->content);
            }
        else {
            store(local_env, ptr1->content, right);
        }
    }

    if (func->yield)
        return pygenerator_init(copy(func->fsuite), local_env);

    execute(func->fsuite, local_env, 0);

    return local_env->ret;
}