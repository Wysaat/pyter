#include "../cpyter.h"

buffer *buff_init() {
    buffer *retptr = (buffer *)malloc(sizeof(buffer));
    memset(retptr, 0, sizeof(buffer));
    return retptr;
}

void buff_del(buffer *buff) {
    buffer *ptr = buff, *tmp;
    while (ptr) {
        tmp = ptr->next;
        if (ptr->value)
            free(ptr->value);
        free(ptr);
        ptr = tmp;
    }
}

void buff_add(buffer *buff, char ch) {
    buffer *ptr = buff;
    while (ptr->next) ptr = ptr->next;
    int len = strlen(ptr->value);
    if (len < BUFF_SZ)
        ptr->value[len] = ch;
    else {
        ptr->next = buff_init();
        buff->value = (char *)malloc(BUFF_SZ+1);
        ptr->next->prev = ptr;
        ptr->next->values[0] = ch;
    }
}

int buff_len(buffer *buff) {
    buffer *ptr = buff;
    int retval;
    while (ptr) {
        retval += strlen(ptr->value);
        ptr = ptr->next;
    }
    return retval;
}