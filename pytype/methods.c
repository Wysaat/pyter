#include <stdlib.h>
#include "../struct_info.h"
#include "../types.h"
#include "pyint.h"
#include "pybool.h"
#include "pyfloat.h"
#include "pycomplex.h"
#include "pylist.h"
#include "pyset.h"
#include "pytuple.h"
#include "pydict.h"
#include "pyfunction.h"
#include "pyclass.h"
#include "py__builtins__.h"
#include "others.h"

pybool *__eq__(void *left, void *right) {
    if (type(left) != type(right))
        return PYBOOL(0);
    int type_n = type(left);
    if (type_n == pyint_t)
        return pyint__eq__(left, right);
    else if (type_n == pystr_t)
        return pystr__eq__(left, right);
    // else if (type_n == pybool_t)
    //     return pybool__eq__(left, right);
    else if (type_n == pylist_t)
        return pylist__eq__(left, right);
    else if (type_n == pytuple_t)
        return pytuple__eq__(left, right);
    // else if (type_n == pyset_t)
    //     return pyset__eq__(left, right);
    // else if (type_n == pydict_t)
    //     return pydict__eq__(left, right);
}

void *__getitem__(void *left, void *right)  {
    if (type(left) == pylist_t) {
        return pylist__getitem__(left, right);
    }
    else if (type(left) == pytuple_t) {
        return pytuple__getitem__(left, right);
    }
    else if (type(left) == pyrange_t) {
        return pyrange__getitem__(left, right);
    }
    else if (type(left) == pystr_t) {
        return pystr__getitem__(left, right);
    }
    else if (type(left) == pydict_t) {
        return pydict__getitem__(left, right);
    }
}

void __setitem__(void *left, void *right, void *value) {
    if (type(left) == pylist_t) {
        pylist__setitem__(left, right, value);
    }
    else if (type(left) == pydict_t) {
        pydict__setitem__(left, right, value);
    }
}

void *__mul__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__mul__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__mul__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__mul__(left, right);
    else if (type(left) == pystr_t)
        return pystr__mul__(left, right);
    else if (type(left) == pylist_t)
        return pylist__mul__(left, right);
    else if (type(left) == pytuple_t)
        return pytuple__mul__(left, right);
}

void *__div__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__div__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__div__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__div__(left, right);
}

void *__rfloordiv__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__rfloordiv__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__rfloordiv__(left, right);
}

void *__add__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__add__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__add__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__add__(left, right);
    else if (type(left) == pystr_t)
        return pystr__add__(left, right);
    else if (type(left) == pylist_t)
        return pylist__add__(left, right);
    else if (type(left) == pytuple_t)
        return pytuple__add__(left, right);
}

void *__sub__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__sub__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__sub__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__sub__(left, right);
}

pybool *__bool__(void *ptr) {
    if (type(ptr) == pyint_t)
        return pyint__bool__(ptr);
    // else if (type(ptr) == pyfloat_t)
    //     return pyfloat__bool__(ptr);
    // else if (type(ptr) == pycomplex_t)
    //     return pycomplex__bool__(ptr);
    else if (type(ptr) == pystr_t)
        return pystr__bool__(ptr);
    else if (type(ptr) == pybool_t)
        return pybool__bool__(ptr);
    // else if (type(ptr) == pylist_t)
    //     return pylist__bool__(ptr);
    // else if (type(ptr) == pytuple_t)
    //     return pytuple__bool__(ptr);
    // else if (type(ptr) == pyset_t)
    //     return pyset__bool__(ptr);
    // else if (tpye(ptr) == pydict_t)
    //     return pydict__bool__(ptr);
}

void *__call__(void *left, void *right) {
    if (type(left) == pyfunction_t)
        return pyfunction__call__(left, right);
    else if (type(left) == pyclass_t)
        return pyclass__call__(left, right);
    else if (type(left) == pybuiltin_function_t)
        return pybuiltin_function__call__(left, right);
}

/*
 * DON'T DELETE IT
 * see how you got wrong
 */
// void *___getattribute__(void *first, void *second, pystr *attr) {
//     if (type(first) == pyclass_t)
//         return pyclass__getattribute__(first, second, attr);
// }

void *__getattribute__(void *first, void *second, char *attr) {
    if (type(first) == pyclass_t)
        return pyclass__getattribute__(first, second, attr);
}

void __setattr__(void *first, void *second, char *attr, void *val) {
    if (type(first) == pyclass_t)
        pyclass__setattr__(first, second, attr, val);
}

pyint *len(void *vptr) {
    if (type(vptr) == pylist_t)
        return pylist__len__(vptr);
    else if (type(vptr) == pytuple_t)
        return pytuple__len__(vptr);
    else if (type(vptr) == pyset_t)
        return pyset__len__(vptr);
    else if (type(vptr) == pystr_t)
        return pystr_len(vptr);
    else if (type(vptr) == pyrange_t)
        return pyrange_len(vptr);
}

pystr *str(void *vptr) {
    if (type(vptr) == pyint_t)
        return pyint__str__(vptr);
    else if (type(vptr) == pyfloat_t)
        return pyfloat__str__(vptr);
    else if (type(vptr) == pycomplex_t)
        return pycomplex__str__(vptr);
    else if (type(vptr) == pystr_t)
        return pystr__str__(vptr);
    else if (type(vptr) == pylist_t)
        return pylist_str(vptr);
    else if (type(vptr) == pytuple_t)
        return pytuple_str(vptr);
    else if (type(vptr) == pyset_t)
        return pyset_str(vptr);
    else if (type(vptr) == pyrange_t)
        return pyrange__str__(vptr);
}

pyfloat *__float__(void *vptr) {
    if (type(vptr) == pyint_t)
        return pyint__float__(vptr);
    else if (type(vptr) == pyfloat_t)
        return pyfloat__float__(vptr);
    // else if (type(vptr) == pystr_t)
    //     return pystr__float__(vptr);
}

pycomplex *py__complex__(void *vptr) {
    if (type(vptr) == pyint_t)
        return pyint__complex__(vptr);
    else if (type(vptr) == pyfloat_t)
        return pyfloat__complex__(vptr);
    else if (type(vptr) == pycomplex_t)
        return pycomplex__complex__(vptr);
}

void *__pow__(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__pow__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__pow__(left, right);
    else if (type(left) == pycomplex_t)
        return pycomplex__pow__(left, right);
}

void *__abs__(void *vptr) {
    if (type(vptr) == pycomplex_t)
        return pycomplex__abs__(vptr);
}

void *_mod_(void *left, void *right) {
    if (type(left) == pyint_t)
        return pyint__mod__(left, right);
    else if (type(left) == pyfloat_t)
        return pyfloat__mod__(left, right);
    else if (type(left) == instance_t) {
        void *func = env_find(((instance *)left)->class->env, "__mod__");
        pyargument *argument = pyargument_init();
        list *value_list = list_node();
        list_append_content(value_list, left);
        list_append_content(value_list, right);
        return __call__(func, argument);
    }
}