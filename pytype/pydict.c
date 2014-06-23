#include "pydict.h"
#include "../list.h"
#include "methods.h"
#include "../struct_info.h"

void *pydict__getitem__(void *left, void *right) {
    pydict *dict = (pydict *)left;
    list *ptr1, *ptr2;
    for (ptr1 = dict->keys, ptr2 = dict->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        if (__eq__(ptr1->content, right))
            return ptr2->content;
    }
}

void pydict__setitem__(void *left, void *right, void *value) {
    pydict *dict = (pydict *)left;
    list *ptr1, *ptr2;
    for (ptr1 = dict->keys, ptr2 = dict->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        if (__eq__(ptr1->content, right)) {
            del(ptr2->content);
            ptr2->content = value;
            ref(value);
            return;
        }
    }
}