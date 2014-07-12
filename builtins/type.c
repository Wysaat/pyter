#include "builtins.h"
#include "../types.h"

void def_type(environment *env) {
    type_class.type = pyclass_t;
    type_class.class = &type_class;
    type_class.id = "type";
    type_class.env = environment_init(0);
    type_class.inheritance = 0;
    store_id(env, "type", &type_class);
}
