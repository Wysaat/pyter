#include <string.h>
#include "../pytype/pylist.h"
#include "../pytype/pytuple.h"
#include "../pytype/pyset.h"
#include "../pytype/pydict.h"
#include "../pytype/pyclass.h"
#include "../pytype/others.h"
#include "../list.h"
#include "../environment.h"
#include "../types.h"
#include "builtins.h"

pylist *list_init(pyargument *argument) {
    if (!argument)
        return pylist__init__();
    void *iterable = argument->value_list->content;
    if (type(iterable) == pylist_t)
        return iterable;
    else if (type(iterable) == pytuple_t) {
        return pylist_init2(((pytuple *)iterable)->values);
    }
    else if (type(iterable) == pyset_t) {
        return pylist_init2(((pyset *)iterable)->values);
    }
    else if (type(iterable) == pydict_t) {
        return pylist_init2(((pydict *)iterable)->keys);
    }
    else if (type(iterable) == pystr_t) {
        list *values = list_node();
        char *string = ((pystr *)iterable)->value;
        int i;
        for (i = 0; i < strlen(string); i++) {
            char val[2];
            val[0] = string[i];
            val[1] = 0;
            list_append_content(values, pystr_init2(val));
        }
        return pylist_init2(values);
    }
}

void *_append(pyargument *argument) {
    list *val = argument->value_list;
    pylist__append__(val->content, val->next->content);
    return pyNone_init();
}

void def_list_func(void *func(), char *name) {
    def_func(list_class.env, func, name);
}

void def_list(environment *env) {
    list_class.type = pyclass_t;
    list_class.class = &type_class;
    list_class.id = "list";
    list_class.env = environment_init(0);
    list_class.inheritance = 0;
    def_list_func(_append, "append");
    store_id(env, "list", &list_class);
}
