#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pytype/py__builtins__.h"
#include "evaluate.h"
#include "environment.h"
#include "types.h"
#include "pytype/pytuple.h"
#include "pytype/pygenerator.h"
#include "pytype/pylist.h"
#include "pytype/pyint.h"
#include "pytype/pybool.h"
#include "pytype/pyclass.h"
#include "__builtins__.h"
#include "pytype/others.h"
#include "pytype/methods.h"
#include "builtins/builtins.h"

void def_range(environment *env) {
    range_class.type = pyclass_t;
    range_class.class = &type_class;
    range_class.id = "range";
    range_class.env = environment_init(0);
    range_class.inheritance = 0;
    store_id(env, "range", &range_class);
}

void def_type(environment *env) {
    type_class.type = pyclass_t;
    type_class.class = &type_class;
    type_class.id = "type";
    type_class.env = environment_init(0);
    type_class.inheritance = 0;
    store_id(env, "type", &type_class);
}

void def__builtins__(environment *env) {
    __builtins__module.type = pymodule_t;
    __builtins__module.name = "__builtins__";
    __builtins__module.env = environment_init(0);
    store_id(env, "__builtins__", &__builtins__module);
    def_func(__builtins__module.env, _print, "print");
    def_func(__builtins__module.env, _next, "next");
    def_func(__builtins__module.env, _len, "len");
    def_func(__builtins__module.env, _abs, "abs");
    def_int(__builtins__module.env);
    def_str(__builtins__module.env);
    def_list(__builtins__module.env);
    def_range(__builtins__module.env);
    def_type(__builtins__module.env);
}