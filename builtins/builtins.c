#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../evaluate.h"
#include "../environment.h"
#include "../types.h"
#include "../pytype/pytuple.h"
#include "../pytype/pygenerator.h"
#include "../pytype/pylist.h"
#include "../pytype/pyint.h"
#include "../pytype/pybool.h"
#include "../pytype/pyclass.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "builtins.h"

void def___builtins__(environment *env) {
    __builtins__module.type = pymodule_t;
    __builtins__module.name = "__builtins__";
    __builtins__module.env = environment_init(0);
    store_id(env, "__builtins__", &__builtins__module);
    def_func(__builtins__module.env, _print, "print");
    def_func(__builtins__module.env, _next, "next");
    def_func(__builtins__module.env, _len, "len");
    def_func(__builtins__module.env, _abs, "abs");
    def_int(__builtins__module.env);
    def_bool(__builtins__module.env);
    def_float(__builtins__module.env);
    def_str(__builtins__module.env);
    def_list(__builtins__module.env);
    store_id(__builtins__module.env, "tuple", &tuple_class);
    def_range(__builtins__module.env);
    def_type(__builtins__module.env);
}

void def_builtins(environment *env) {
    def_func(env, _print, "print");
    def_func(env, _next, "next");
    def_func(env, _len, "len");
    def_func(env, _abs, "abs");
    def_int(env);
    def_bool(env);
    def_float(env);
    def_str(env);
    def_list(env);
    def_tuple(env);
    def_range(env);
    def_type(env);
    def___builtins__(env);
}