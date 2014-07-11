#include "../types.h"
#include "../pytype/pybool.h"
#include "builtins.h"

pybool *bool_init(pyargument *argument) {}

void def_func(environment *env, void *func(), char *name);

void def_bool_func(void *func(), char *name) {
    def_func(bool_class.env, func, name);
}

void def_bool() {
    bool_class.type = pyclass_t;
    bool_class.class = &type_class;
    bool_class.id = "bool";
    bool_class.env = environment_init(0);
    bool_class.inheritance = 0;
    store_id(env, "bool", &bool_class);
}