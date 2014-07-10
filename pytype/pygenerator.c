#include <stdlib.h>
#include <string.h>
#include "pygenerator.h"
#include "../types.h"
#include "../__builtins__.h"
#include "../gen_execute.h"
#include "../environment.h"

pygenerator *pygenerator_init(void *_suite, environment *local_env, char *id) {
    pygenerator *retptr = (pygenerator *)malloc(sizeof(pygenerator));
    memset(retptr, 0, sizeof(retptr));
    retptr->type = pygenerator_t;
    retptr->class = &generator_class;
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
