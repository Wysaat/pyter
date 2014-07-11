/*
 * CAUTION ! If you forget to include <stdlib.h> and use atof,
 * gcc will not give any error message or warning if you
 * didn't use something like -Wall, but atof will always
 * give a result of 0.
 */
#include <stdlib.h>
#include "../types.h"
#include "../pytype/pyfloat.h"
 #include "builtins.h"

pyfloat *float_init(pyargument *argument) {
    if (!argument)
        return pyfloat__init__();
    void *ptr = argument->value_list->content;
    if (type(ptr) == pyint_t)
        return pyint__float__(ptr);
    else if (type(ptr) == pyfloat_t)
        return pyfloat__float__(ptr);
    else if (type(ptr) == pystr_t)
        return pyfloat_init2(atof(((pystr *)ptr)->value));
}

void def_func(environment *env, void *func(), char *name);

void def_float_func(void *func(), char *name) {
    def_func(float_class.env, func, name);
}

void def_float(environment *env) {
    float_class.type = pyclass_t;
    float_class.class = &type_class;
    float_class.id = "float";
    float_class.env = environment_init(0);
    float_class.inheritance = 0;
    store_id(env, "float", &float_class);
}