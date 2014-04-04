#include "../types.h"
#include "../execute.h"
#include "pyint.h"
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