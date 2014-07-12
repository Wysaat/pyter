#include "builtins.h"
#include "../types.h"

void def_range(environment *env) {
    range_class.type = pyclass_t;
    range_class.class = &type_class;
    range_class.id = "range";
    range_class.env = environment_init(0);
    range_class.inheritance = 0;
    store_id(env, "range", &range_class);
}
