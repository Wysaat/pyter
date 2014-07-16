#include "../types.h"
#include "../pytype/pybool.h"
#include "../pytype/pyint.h"
#include "../pytype/pyfloat.h"
#include "../pytype/pycomplex.h"
#include "../pytype/pylist.h"
#include "../pytype/pytuple.h"
#include "../pytype/pyset.h"
#include "../pytype/pydict.h"
#include "../pytype/methods.h"
#include "builtins.h"

pybool *bool_init(pyargument *argument) {
    if (!argument)
        return PYBOOL(0);
    void *ptr = argument->value_list->content;
    if (type(ptr) == pyint_t) {
        if (pyint_iszero(ptr))
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pybool_t)
        return ptr;
    else if (type(ptr) == pyfloat_t) {
        pyfloat *fptr = (pyfloat *)ptr;
        if (fptr->value == 0)
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pycomplex_t) {
        pycomplex *cptr = (pycomplex *)ptr;
        if (cptr->real->value == 0 && cptr->imag->value == 0)
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pylist_t) {
        pylist *lp = (pylist *)ptr;
        if (list_is_empty(lp->values))
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pyset_t) {
        pyset *sp = (pyset *)ptr;
        if (list_is_empty(sp->values))
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pytuple_t) {
        pytuple *sp = (pytuple *)ptr;
        if (list_is_empty(sp->values))
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pydict_t) {
        pydict *sp = (pydict *)ptr;
        if (list_is_empty(sp->keys))
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == pyrange_t) {
        if (pyrange_len2(ptr) == 0)
            return PYBOOL(0);
        return PYBOOL(1);
    }
    else if (type(ptr) == instance_t) {
        void *func = env_find(((instance *)ptr)->class->env, "__bool__");
        return __call__(func, argument);
    }
}

void def_func(environment *env, void *func(), char *name);

void def_bool_func(void *func(), char *name) {
    def_func(bool_class.env, func, name);
}

void def_bool(environment *env) {
    bool_class.type = pyclass_t;
    bool_class.class = &type_class;
    bool_class.id = "bool";
    bool_class.env = environment_init(0);
    bool_class.inheritance = 0;
    store_id(env, "bool", &bool_class);
}