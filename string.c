/* premature optimization is the root of all evil */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

char *stradd(char *left, char *right) {
    char *retval = (char *)malloc(strlen(left)+strlen(right)+1);
    strcpy(retval, left);
    strcpy(retval+strlen(left), right);
    return retval;
}

char *stradd2(char *left, char *right) {
    char *retval = (char *)malloc(strlen(left)+strlen(right)+1);
    strcpy(retval, left);
    strcpy(retval+strlen(left), right);
    free(left);
    free(right);
    return retval;
}

char *strslice(char *val, int start, int stop, int step) {
    if (start < 0) start += strlen(val);
    if (stop < 0) stop += strlen(val);

    int len = (stop-start)/step, i;
    if ((stop-start) % step)
        len++;
    if (len <= 0) len = 0;
    char *retval = (char *)malloc(len+1);
    retval[len] = 0;
    for (i = 0; i < len; i++)
        retval[i] = val[start+step*i];

    return retval;
}

buffer2 *buff2_init() {
    buffer2 *retptr = (buffer2 *)malloc(sizeof(buffer2));
    retptr->prev = 0;
    retptr->next = 0;
    return retptr;
}

void *buff2_add(buffer2 *buff, char *value) {
    buffer2 *ptr = buff;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = buff2_init();
    ptr->next->content = value;
}

char *buff2_puts(buffer2 *buff) {
    buffer2 *ptr = buff->next;
    int len = 0;
    while (ptr) {
        len += strlen(ptr->content);
        ptr = ptr->next;
    }
    char *retptr = (char *)malloc(sizeof(char)*(len+1));
    int offs = 0;
    for (ptr = buff->next; ptr; ptr = ptr->next) {
        strcpy(retptr+offs, ptr->content);
        offs += strlen(ptr->content);
    }
    retptr[len] = 0;
    return retptr;
}

/* don't delete it, see how you got wrong, (hint: next) */
// void buff2_del(buffer2 *buff) {
//     buffer2 *ptr, *tmp;
//     for (ptr = buff->next; ptr; ptr = ptr->next) {
//         free(ptr->content);
//         tmp = ptr->next;
//         free(ptr);
//         ptr = tmp;
//     }
//     free(buff);
// }

void buff2_del(buffer2 *buff) {
    buffer2 *ptr, *tmp;
    while (ptr) {
        free(ptr->content);
        tmp = ptr->next;
        free(ptr);
        ptr = tmp;
    }
    free(buff);
}

char *buff2_puts2(buffer2 *buff) {
    char *retptr = buff2_puts(buff);
    buff2_del(buff);
    return retptr;
}
