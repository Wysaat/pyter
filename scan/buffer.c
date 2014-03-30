#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

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
    if (*(int *)buff == 0) {
        buff->value = (char *)malloc(BUFF_SZ+1);
        memset(buff->value, 0, BUFF_SZ+1);
        buff->value[0] = ch;
        return;
    }
    while (ptr->next) ptr = ptr->next;
    int len = strlen(ptr->value);
    if (len < BUFF_SZ)
        ptr->value[len] = ch;
    else {
        ptr->next = buff_init();
        ptr->next->value = (char *)malloc(BUFF_SZ+1);
        memset(ptr->next->value, 0, BUFF_SZ+1);
        ptr->next->prev = ptr;
        ptr->next->value[0] = ch;
    }
}

int buff_len(buffer *buff) {
    if (!buff->value)
        return 0;
    buffer *ptr = buff;
    int retval = 0;
    while (ptr) {
        retval += strlen(ptr->value);
        ptr = ptr->next;
    }
    return retval;
}

char *buff_puts(buffer *buff) {
    char *retval = (char *)malloc(buff_len(buff)+1);
    buffer *ptr = buff;
    int offset = 0;
    while (ptr) {
        strcpy(retval+offset, ptr->value);
        offset += strlen(ptr->value);
        ptr = ptr->next;
    }
    return retval;
}