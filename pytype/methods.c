#include "../types.h"
#include "../execute.h"
#include "pyint.h"
#include "pyfloat.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pybool.h"
#include "pylist.h"
#include "pytuple.h"
#include "pyset.h"
#include "pydict.h"
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