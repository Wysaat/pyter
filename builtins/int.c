#include "../pytype/pyint.h"
#include "../pytype/pyfloat.h"
#include "../pytype/py__builtins__.h"
#include "../pytype/others.h"
#include "../types.h"
#include "../__builtins__.h"

pyint *int_init(pyargument *argument) {
    if (!argument)
        return int_to_pyint(0);
    if (argument->assign_target_list) {
    }
    else {
        void *vptr = argument->value_list->content;
        switch (type(vptr)) {
            case pyint_t:
                return vptr;
            case pyfloat_t:
                return pyfloat__int__(vptr);
        }
    }
}

void def_func(environment *env, void *func(), char *name);

void def_int_func(void *func(), char *name) {
    def_func(int_class.env, func, name);
}

void def_int(environment *env) {
    int_class.type = pyclass_t;
    int_class.ref = 0;
    int_class.class = &type_class;
    int_class.id = "int";
    int_class.env = environment_init(0);
    int_class.inheritance = 0;
    store_id(env, "int", &int_class);
}
