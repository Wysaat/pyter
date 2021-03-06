#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../types.h"
#include "../struct_info.h"
#include "pyint.h"
#include "../integer.h"
#include "pybool.h"
#include "pycomplex.h"
#include "pystr.h"
#include "pylist.h"
#include "pytuple.h"
#include "methods.h"
#include "../builtins/builtins.h"

pyint *pyint__init__() {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    memset(retptr, 0, sizeof(pyint));
    retptr->type = pyint_t;
    retptr->class = &int_class;
    return retptr;
}

pyint *pyint_init2(void *x, pyint *base) {
    pyint *order = int_to_pyint(1);
    pyint *val, *retptr;

    char *string, *ptr;
    switch (type(x)) {
        case pyint_t:
            return x;
        case pyfloat_t:
            retptr = pyfloat__int__(x);
            return retptr;
        case pystr_t:
            string = ((pystr *)x)->value;
            while (*string == ' ' || *string == '\t' || *string == '\n')
                string++;
            if (*string == '0') {
                if (*(string+1) == 'b' || *(string+1) == 'B') {
                    base = int_to_pyint(2);
                    string += 2;
                }
                else if (*(string+1) == 'o' || *(string+1) == 'O') {
                    base = int_to_pyint(8);
                    string += 2;
                }
                else if (*(string+1) == 'x' || *(string+1) == 'X') {
                    base = int_to_pyint(16);
                    string += 2;
                }
            }
            if (pyint_to_int(base) == 0) {
                base = int_to_pyint(10);
            }
            ptr = string;
            while (*ptr != 0 && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
                ptr++;
            ptr--;
            retptr = int_to_pyint(0);
            for ( ; ptr != string-1; ptr--) {
                if (*ptr >= '0' && *ptr <= '9')
                    val = pyint__mul__(int_to_pyint(*ptr - '0'), order);
                else if (*ptr >= 'a' && *ptr <= 'z')
                    val = pyint__mul__(int_to_pyint(*ptr - 'a' + 10), order);
                else if (*ptr >= 'A' && *ptr <= 'Z')
                    val = pyint__mul__(int_to_pyint(*ptr - 'A' + 10), order);
                order = pyint__mul__(order, base);
                retptr = pyint__add__(retptr, val);
            }

            return retptr;
    }
}

void pyint_del2(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    integer__del__(ptr->value);
    free(ptr);
}

pybool *pyint__bool__(void *vptr) {
    pybool *retptr;
    if (pyint__eq__(vptr, int_to_pyint(0)))
        retptr = PYBOOL(0);
    else
        retptr = PYBOOL(1);
    return retptr;
}

void *pyint__add__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__add__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pycomplex_t) {
        void *retptr = pyfloat__add__(pyint__float__(left), rvoid);
        return retptr;
    }
}

void *pyint_add2(void *lvoid, void *rvoid) {
    pyint *retptr = pyint__add__(lvoid, rvoid);
    pyint_del2(lvoid);
    return retptr;
}

void *pyint_add3(void *lvoid, void *rvoid) {
    pyint *retptr = pyint__add__(lvoid, rvoid);
    pyint_del2(lvoid);
    pyint_del2(rvoid);
    return retptr;
}

void *pyint__sub__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__sub__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t || type(rvoid) == pycomplex_t) {
        void *retptr = pyfloat__sub__(pyint__float__(left), rvoid);
        return retptr;
    }
}

void *pyint__mul__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__mul__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t)
        return pyfloat__mul__(rvoid, lvoid);
    else if (type(rvoid) == pycomplex_t)
        return pycomplex__mul__(rvoid, lvoid);
    else if (type(rvoid) == pystr_t)
        return pystr__mul__(rvoid, lvoid);
    else if (type(rvoid) == pylist_t)
        return pylist__mul__(rvoid, lvoid);
    else if (type(rvoid) == pytuple_t)
        return pytuple__mul__(rvoid, lvoid);
}

void *pyint_mul2(void *lvoid, void *rvoid) {
    pyint *retptr = pyint__mul__(lvoid, rvoid);
    pyint_del2(lvoid);
    return retptr;
}

void *pyint_mul3(void *lvoid, void *rvoid) {
    pyint *retptr = pyint__mul__(lvoid, rvoid);
    pyint_del2(lvoid);
    pyint_del2(rvoid);
    return retptr;
}

void *pyint__div__(void *lvoid, void *rvoid) {
    void *retptr = pyfloat__div__(pyint__float__(lvoid), rvoid);
    return retptr;
}

void *pyint__rfloordiv__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyint *left = (pyint *)lvoid;
        pyint *right = (pyint *)rvoid;
        pyint *retptr = pyint__init__();
        retptr->value = integer__div__(left->value, right->value);
        return retptr;
    }
    pyfloat *result = pyint__div__(lvoid, rvoid);
    result->value = floor(result->value);
    return result;
}

void *pyint__mod__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        pyint *retptr = pyint__init__();
        pyint *right = (pyint *)rvoid;
        retptr->value = integer__mod__(left->value, right->value);
        return retptr;
    }
    else if (type(rvoid) == pyfloat_t) {
        pyfloat *retptr = pyfloat__mod__(pyint__float__(left), rvoid);
        return retptr;
    }
}

pyint *pyint__lshift__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__lshift__(left->value, right->value);
    return retptr;
}

pyint *pyint__rshift__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__rshift__(left->value, right->value);
    return retptr;
}

void pyint__print__(pyint *val) {
    char *string = integer__str__(val->value);
    printf("%s", string);
    free(string);
}

void pyint__puts__(pyint *val) {
    char *string = integer__str__(val->value);
    puts(string);
    free(string);
}

void pyint__dec__(pyint *val) {
    integer *new_value = integer__dec__(val->value);
    integer__del__(val->value);
    val->value = new_value;
}

void pyint__inc__(pyint *val) {
    integer *new_value = integer__inc__(val->value);
    integer__del__(val->value);
    val->value = new_value;
}

pyint *int_to_pyint(int number) {
    pyint *retptr = pyint__init__();
    char sign = '+';
    if (number < 0) {
        number = -number;
        sign = '-';
    }
    retptr->value = integer__init__(itoa(number));
    retptr->value->sign = sign;
    return retptr;
}

int pyint_to_int(pyint *ptr) {
    return (atoi(integer__str__(ptr->value)));
}

pyint *pyint_cpy(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    pyint *retptr = pyint__init__();
    retptr->value = integer__cpy__(ptr->value);
    return retptr;
}

pyint *pyint__neg__(pyint *ptr) {
    pyint *retptr = pyint_cpy(ptr);
    if (retptr->value->sign == '+')
        retptr->value->sign = '-';
    else
        retptr->value->sign = '+';
    return retptr;
}

pyint *pyint__invert__(pyint *ptr) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__invert__(ptr->value);
    return retptr;
}

void *pyint__pow__(void *lvoid, void *rvoid) {
    if (type(rvoid) == pyint_t) {
        pyint *left = (pyint *)lvoid;
        pyint *right = (pyint *)rvoid;
        if (right->value->sign == '-') {
            pyfloat *fleft = pyint__float__(lvoid);
            pyfloat *fright = pyint__float__(rvoid);
            void *retptr = pyfloat__pow__(fleft, fright);
            return retptr;
        }
        else {
            pyint *retptr = pyint__init__();
            retptr->value = integer__pow__(left->value, right->value);
            return retptr;
        }
    }
    else if (type(rvoid) == pyfloat_t) {
        pyfloat *fptr = pyint__float__(lvoid);
        void *retptr = pyfloat__pow__(fptr, rvoid);
        return retptr;
    }
    else if (type(rvoid) == pycomplex_t) {
        pycomplex *cptr = pyint__complex__(lvoid);
        void *retptr = pycomplex__pow__(cptr, rvoid);
        return retptr;
    }
}

pyint *pyint__int__(void *vptr) {
    return (pyint *)vptr;
}

pyfloat *pyint__float__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    pyfloat *retptr = pyfloat__init__();
    integer *integerp = intptr->value;
    while (integerp) {
        retptr->value += integerp->value * pow(1e1, INTEGER_SZ*integerp->index);
        integerp = integerp->lower;
    }
    if (intptr->value->sign == '-')
        retptr->value = -retptr->value;
    return retptr;
}

pycomplex *pyint__complex__(void *vptr) {
    pyint *intptr = (pyint *)vptr;
    pycomplex *retptr = pycomplex__init__();
    integer *integerp = intptr->value;
    while (integerp) {
        retptr->real->value += integerp->value * pow(1e1, INTEGER_SZ*integerp->index);
        integerp = integerp->lower;
    }
    if (intptr->value->sign == '-')
        retptr->real->value = -retptr->real->value;
    return retptr;
}

pystr *pyint__str__(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    char *value = integer__str__(ptr->value);
    pystr *retptr = pystr_init2(value);
    free(value);
    return retptr;
}

pyint *pyint__abs__(void *vptr) {
    pyint *ptr = (pyint *)vptr;
    ptr->value->sign = '+';
    return ptr;
}

int pyint_iszero(pyint *ptr) {
    integer *integerp = ptr->value;
    return (!integerp->higher && !integerp->lower && integerp->value == 0);
}

pybool *pyint__lt__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t)
        return PYBOOL(integer__lt__(left->value, ((pyint *)rvoid)->value));
    else if (type(rvoid) == pyfloat_t)
        return pyfloat__lt__(pyint__float__(lvoid), rvoid);
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyint__lt__(left, int_to_pyint(right->value));
    }
}

pybool *pyint__gt__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t)
        return PYBOOL(integer__gt__(left->value, ((pyint *)rvoid)->value));
    else if (type(rvoid) == pyfloat_t)
        return pyfloat__gt__(pyint__float__(lvoid), rvoid);
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyint__gt__(left, int_to_pyint(right->value));
    }
}

pybool *pyint__eq__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    if (type(rvoid) == pyint_t) {
        return PYBOOL(integer__eq__(left->value, ((pyint *)rvoid)->value));
    }
    else if (type(rvoid) == pyfloat_t) {
        return pyfloat__eq__(pyint__float__(left), rvoid);
    }
    else if (type(rvoid) == pybool_t) {
        pybool *right = rvoid;
        return pyint__eq__(left, int_to_pyint(right->value));
    }
    return PYBOOL(0);
}

pybool *pyint__ge__(void *lvoid, void *rvoid) {
    pyint *left = (pyint *)lvoid;
    pyint *right = (pyint *)rvoid;
    if (integer__ge__(left->value, right->value))
        return PYBOOL(1);
    return PYBOOL(0);
}

int pyint__cmp__(pyint *left, pyint *right) {
    return integer__cmp__(left->value, right->value);
}
