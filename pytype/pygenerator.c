#include <stdlib.h>
#include <string.h>
#include "pygenerator.h"
#include "../types.h"
#include "../gen_execute.h"

pygenerator *pygenerator_init(void *_suite, environment *local_env, char *id) {
    pygenerator *retptr = (pygenerator *)malloc(sizeof(pygenerator));
    memset(retptr, 0, sizeof(retptr));
    retptr->type = pygenerator_t;
    retptr->_suite = _suite;
    retptr->local_env = local_env;
    retptr->id = id;
    return retptr;
}

pygenerator *pygenerator_next(void *vptr) {
    pygenerator *gen = (pygenerator *)vptr;
    gen->local_env->yield = 0;
    gen_execute(gen->_suite, gen->local_env, 0);
    return gen->local_env->yield;
}

// pygenerator *pygenerator_send(void *vptr, void * val) {
//     pygenerator *gen = (pygenerator *)vptr;
//     gen->local_env->yield = 0;
//     gen->local_env->send = val;
//     execute(gen->_suite, gen->local_env, 0);
//     gen->local_env->send = 0;
// }