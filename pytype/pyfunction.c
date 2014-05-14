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
    if (func->assign_target_list)
        store(local_env, func->assign_target_list, func->assign_values);

    ptr1 = func->parameters;
    if (func->bound) {
        store(local_env, func->parameters->content, func->bound);
        ptr1 = func->parameters->next;
    }
    if (ptr1 && ptr1->content && right) {
        if (type(right) == pytuple_t)
            for (ptr2 = ((pytuple *)right)->values;
                 ptr1 && ptr2; ptr1 = ptr1->next, ptr2 = ptr2->next) {
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