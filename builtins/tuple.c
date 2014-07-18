#include "builtins.h"
#include "../types.h"

pybool *tuple__gt__(pyargument *argument) {
}

void def_tuple(environment *env) {
    tuple_class.type = pyclass_t;
    tuple_class.class = &type_class;
    tuple_class.id = "tuple";
    tuple_class.env = environment_init(0);
    tuple_class.inheritance = 0;
    store_id(env, "tuple", &tuple_class);
}