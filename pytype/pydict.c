#include <stdlib.h>
#include "pydict.h"
#include "others.h"
#include "../list.h"
#include "methods.h"
#include "../struct_info.h"
#include "../types.h"
#include "../builtins/builtins.h"

pydict *pydict_init() {
    pydict *retptr = (pydict *)malloc(sizeof(pydict));
    retptr->type = pydict_t;
    retptr->class = &dict_class;
    return retptr;
}

void *pydict__getitem__(void *left, void *right) {
    pydict *dict = (pydict *)left;
    list *ptr1, *ptr2;
    for (ptr1 = dict->keys, ptr2 = dict->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        if (__eq__(pyargument_init3(ptr1->content, right)))
            return ptr2->content;
    }
}

void pydict__setitem__(void *left, void *right, void *value) {
    pydict *dict = (pydict *)left;
    list *ptr1, *ptr2;
    for (ptr1 = dict->keys, ptr2 = dict->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        if (__eq__(pyargument_init3(ptr1->content, right))) {
            ptr2->content = value;
            return;
        }
    }
}
