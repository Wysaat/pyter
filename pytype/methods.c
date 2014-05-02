#include "../types.h"
#include "../evaluate.h"
#include "pyint.h"
#include "pyfloat.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pybool.h"
#include "pylist.h"
#include "pytuple.h"
#include "pyset.h"
#include "pydict.h"
#include "pyfunction.h"
#include "pyclass.h"
#include "py__builtins__.h"
#include <stdio.h>

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
}

void __setitem__(void *left, void *right, void *value) {
    if (type(left) == pylist_t) {
        pylist__setitem__(left, right, value);
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

void *__getattribute__(void *first, void *second, pystr *attr) {
    if (type(first) == pyclass_t)
        return pyclass__getattribute__(first, second, attr);
}

void __setattr__(void *first, void *second, pystr *attr, void *val) {
    puts("entering __setattr__");
    printf("type(first) is %d\n", type(first));
    if (type(first) == pyclass_t)
        pyclass__setattr__(first, second, attr, val);
}