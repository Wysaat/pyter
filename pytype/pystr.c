#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "pystr.h"
#include "../struct_info.h"
#include "pyint.h"
#include "pybool.h"
#include "others.h"
#include "../string.h"
#include "../builtins/builtins.h"

pystr *pystr__init__() {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = strdup("");
    retptr->type = pystr_t;
    retptr->class = &str_class;
    return retptr;
}

pystr *pystr_init2(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = strdup(value);
    retptr->type = pystr_t;
    retptr->class = &str_class;
    return retptr;
}

pystr *pystr_init3(char *value) {
    pystr *retptr = (pystr *)malloc(sizeof(pystr));
    retptr->value = value;
    retptr->type = pystr_t;
    retptr->class = &str_class;
    return retptr;
}

pystr *pystr__mul__(void *left, void *right) {
    pystr *strptr = (pystr *)left, *retptr = pystr__init__(), *new_retptr;
    pyint *times = (pyint *)right;
    pyint *zero = pyint__init__();
    zero->value = INTEGER_NODE();
    while (is_true(pyint__gt__(times, zero))) {
        new_retptr = pystr__add__(retptr, strptr);
        retptr = new_retptr;
        pyint__dec__(times);
    }
    return retptr;
}

pystr *pystr__add__(void *left, void *right) {
    pystr *leftptr = (pystr *)left;
    pystr *rightptr = (pystr *)right;
    pystr *retptr = pystr__init__();
    retptr->value = stradd(leftptr->value, rightptr->value);
    return retptr;
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
            index += (int)strlen(primary->value);
        char val[2];
        val[0] = primary->value[index];
        val[1] = 0;
        return pystr_init2(val);
    }
    else if (type(right) == pyslice_t) {
        pyslice *slice = (pyslice *)right;
        int length = (int)strlen(primary->value);
        int start, stop, step;
        step = slice->step;
        if (slice->nostart) {
            if (slice->step > 0)
                start = 0;
            else
                start = length - 1;
        }
        else {
            start = slice->start;
            if (start < 0)
                start += length;
            else if (step < 0 && start >= length)
                start = length - 1;
        }
        if (slice->nostop) {
            if (slice->step > 0)
                stop = length;
            else
                stop = -1;
        }
        else {
            stop = slice->stop;
            if (stop < 0)
                stop += length;
            else if (step > 0 && stop > length)
                stop = length;
        }
        if (step > 0 && start >= stop)
            return pystr__init__();
        if (step < 0 && start <= stop)
            return pystr__init__();
        int ret_len;
        if ((stop-start)%step == 0)
            ret_len = (stop-start) / step;
        else
            ret_len = (stop-start)/step + 1;
        char val[ret_len+1];
        val[ret_len] = 0;
        int i, j;
        if (step > 0) {
            for (i = start, j = 0; i < stop; i += step, j++)
                val[j] = primary->value[i];
        } else {
            for (i = start, j = 0; i > stop; i += step, j++)
                val[j] = primary->value[i];
        }
        return pystr_init2(val);
    }
}

pystr *pystr__str__(void *vptr) {
    return (pystr *)vptr;
}

pystr *pystr_capitalize(pystr *ptr) {
    int len = strlen(ptr->value);
    char cap[len+1];
    strcpy(cap, ptr->value);
    cap[len] = 0;
    if (cap[0] >= 'a' && cap[0] <= 'z')
        cap[0] += 'A' - 'a';
    return pystr_init2(cap);
}

pystr *pystr_casefold(pystr *ptr) {
    int len = strlen(ptr->value);
    char folded[len+1];
    int i;
    for (i = 0; i < len; i++) {
        folded[i] = ptr->value[i];
        if (ptr->value[i] >= 'A' && ptr->value[i] <= 'Z')
            folded[i] += 'a' - 'A';
    }
    folded[len] = 0;
    return pystr_init2(folded);
}

pystr *pystr_center(pystr *ptr, pyint *width, pystr *fillchar) {
    int wid = pyint_to_int(width);
    char fill;
    if (fillchar)
        fill = *fillchar->value;
    else
        fill = ' ';
    int len = strlen(ptr->value);
    if (wid <= len)
        return ptr;
    else {
        char centered[wid+1];
        int delt = wid - len;
        int front, end;
        end = delt / 2;
        front = delt - end;
        int i;
        for (i = 0; i < front; i++)
            centered[i] = fill;
        for ( ; i < wid - end; i++)
            centered[i] = ptr->value[i-front];
        for ( ; i < wid; i++)
            centered[i] = fill;
        centered[wid] = 0;
        return pystr_init2(centered);
    }
}

int find_subs(char *string, char *subs) {
    int i, j = 0;
    int len = strlen(string), slen = strlen(subs);
    for (i = 0; i < len; i++) {
        if (string[i] == subs[j])
            j++;
        else
            j = 0;
        if (j == slen)
            return i - slen + 1;
    }
    return -1;
}

pyint *pystr_count(pystr *ptr, pystr *sub, pyint *start, pyint *end) {
    int _start, _end;
    if (start)
        _start = pyint_to_int(start);
    else
        _start = 0;
    if (end)
        _end = pyint_to_int(end);
    else
        _end = strlen(ptr->value);
    if (_end < 0)
        _end += strlen(ptr->value);
    int count = 0, incv;
    int slen = strlen(sub->value), len = strlen(ptr->value);
    char *string = strdup(ptr->value);
    if (_end < len)
        string[_end] = 0;
    len = strlen(string);
    while (1) {
        if ((incv = find_subs(string+_start, sub->value)) >= 0) {
            _start += incv;
            count += 1;
            _start += slen;
            if (_start >= len) {
                free(string);
                return int_to_pyint(count);
            }
        }
        else {
            free(string);
            return int_to_pyint(count);
        }
    }
}

pybool *pystr__gt__(void *lvoid, void *rvoid) {
    pystr *left = (pystr *)lvoid;
    if (type(rvoid) == pystr_t) {
        pystr *right = (pystr *)rvoid;
        if (strcmp(left->value, right->value) > 0)
            return PYBOOL(1);
        return PYBOOL(0);
    }
}

pybool *pystr__lt__(void *lvoid, void *rvoid) {
    pystr *left = (pystr *)lvoid;
    if (type(rvoid) == pystr_t) {
        pystr *right = (pystr *)rvoid;
        if (strcmp(left->value, right->value) < 0)
            return PYBOOL(1);
        return PYBOOL(0);
    }
}

pybool *pystr__eq__(void *lvoid, void *rvoid) {
    pystr *left = (pystr *)lvoid;
    if (type(rvoid) == pystr_t) {
        pystr *right = (pystr *)rvoid;
        if (strcmp(left->value, right->value) == 0)
            return PYBOOL(1);
        return PYBOOL(0);
    }
    return PYBOOL(0);
}
