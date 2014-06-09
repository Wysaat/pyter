#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "pystr.h"
#include "pyint.h"
#include "pybool.h"
#include "pylist.h"
#include "others.h"
#include "../string.h"

pystr *pystr__init__() {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = strdup("");
    retptr->type = pystr_t;
    return retptr;
}

pystr *pystr_init2(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = strdup(value);
    retptr->type = pystr_t;
    return retptr;
}

void pystr__del__(pystr *strptr) {
    free(strptr->value);
    free(strptr);
}

pystr *pystr__mul__(void *left, void *right) {
    pystr *strptr = (pystr *)left, *retptr = pystr__init__(), *new_retptr;
    pyint *times = (pyint *)right;
    pyint *zero = pyint__init__();
    zero->value = INTEGER_NODE();
    while (is_true(pyint__gt__(times, zero))) {
        new_retptr = pystr__add__(retptr, strptr);
        pystr__del__(retptr);
        retptr = new_retptr;
        pyint__dec__(times);
    }
    pyint__del__(zero);
    return retptr;
}

pystr *pystr__add__(void *left, void *right) {
    pystr *leftptr = (pystr *)left;
    pystr *rightptr = (pystr *)right;
    pystr *retptr = pystr__init__();
    retptr->value = stradd(leftptr->value, rightptr->value);
    return retptr;
}

pybool *pystr__eq__(void *lvoid, void *rvoid) {
    pystr *left = (pystr *)lvoid;
    pystr *right = (pystr *)rvoid;
    if (!strcmp(left->value, right->value))
        return PYBOOL(1);
    return PYBOOL(0);
}

pybool *pystr__bool__(void *ptr) {
    pystr *strptr = (pystr *)ptr;
    return PYBOOL(strlen(strptr->value));
}


void pystr__print__(pystr *strptr) {
    printf("%s", strptr->value);
}

void pystr__puts__(pystr *strptr) {
    puts(strptr->value);
}

pystr *str_to_pystr(char *str) {
    pystr *retptr = pystr__init__();
    retptr->value = strdup(str);
    return retptr;
}

char *pystr_to_str(pystr *ptr) {
    return ptr->value;
}

void pystr_print_nnl(pystr *sptr) {
    char *ptr = sptr->value;
    printf("'");
    while (*ptr) {
        if (*ptr == '\\')
            printf("\\\\");
        else if (*ptr == '\n')
            printf("\\n");
        else if (*ptr == '\r')
            printf("\\r");
        else if (*ptr == '\t')
            printf("\\t");
        else
            printf("%c", *ptr);
        ptr++;
    }
    printf("'");
}

pyint *pystr_len(void *vptr) {
    pystr *ptr = (pystr *)vptr;
    return int_to_pyint(strlen(ptr->value));
}

pystr *pystr__getitem__(void *left, void *right) {
    pystr *primary = (pystr *)left;
    if (type(right) == pyint_t) {
        int index = pyint_to_int(right);
        if (index < 0)
            index += strlen(primary->value);
        char val[2];
        val[0] = primary->value[index];
        val[1] = 0;
        return pystr_init2(val);
    }
    else if (type(right) == pyslice_t) {
        pyslice *slice = (pyslice *)right;
        int length = strlen(primary->value);
        int start = slice->start;
        int stop;
        if (slice->nostop || slice->stop > strlen(primary->value))
            stop = strlen(primary->value);
        else
            stop = slice->stop;
        int step = slice->step;
        if (start < 0)
            start += length;
        if (stop < 0)
            stop += length;
        if (start >= stop)
            return pystr__init__();
        int ret_len;
        if ((start-stop)%step == 0)
            ret_len = (stop-start) / step;
        else
            ret_len = (stop-start)/step + 1;
        char val[ret_len+1];
        val[ret_len] = 0;
        int i, j;
        for (i = start, j = 0; i < stop; i += step, j++)
            val[j] = primary->value[i];
        return pystr_init2(val);
    }
}