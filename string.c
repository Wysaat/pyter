/* premature optimization is the root of all evil */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stradd(char *left, char *right) {
    char *retval = (char *)malloc(strlen(left)+strlen(right)+1);
    strcpy(retval, left);
    strcpy(retval+strlen(left), right);
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