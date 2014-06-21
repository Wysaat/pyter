#include <string.h>
#include "../pytype/pystr.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../evaluate.h"
#include "../__builtins__.h"

pystr *str_init(pyargument *argument) {
    if (argument->assign_target_list) {
        char *first = ((identifier *)argument->assign_target_list->content)->value;
        if (!strcmp(first, "object")) {
            return str(argument->assign_value_list->content);
        }
    }
    else {
        return str(argument->value_list->content);
    }
}

void *str_capitalize(pyargument* argument) {
    list *val = argument->value_list;
    return pystr_capitalize(val->content);
}

void *def_str_capitalize_func(environment *env) {
    pybuiltin_function *str_capitalize_func = pybuiltin_function__init__("capitalize", str_capitalize);
    store_id(env, "capitalize", str_capitalize_func);
}

void def_str(environment *env) {
    str_class.type = pyclass_t;
    str_class.ref = 0;
    str_class.class = &type_class;
    str_class.id = "str";
    str_class.env = environment_init(0);
    str_class.inheritance = 0;
    def_str_capitalize_func(str_class.env);
    store_id(env, "str", &str_class);
}