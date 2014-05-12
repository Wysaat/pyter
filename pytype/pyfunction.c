#include "pyfunction.h"
#include "../types.h"
#include "pytuple.h"
#include "../execute.h"
#include "../evaluate.h"
#include "../gen_execute.h"
#include "pygenerator.h"

// void *pyfunction__init__(identifier *id, list *parameters, void *fsuite, environment *env) {
//     pyfunction *retptr = (pyfunction *)malloc(sizeof(pyfunction));
//     retptr->type = pyfunction_t;
//     retptr->id = id;
//     retptr->parameters = parameters;
//     retptr->fsuite = fsuite;
//     retptr->env = env;
//     retptr->bound = 0;
//     return retptr;
// }

void *pyfunction__call__(void *lptr, void *right) {
    pyfunction *func = (pyfunction *)lptr;
    environment *local_env = environment_init(func->env);
    list *ptr1, *ptr2;

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