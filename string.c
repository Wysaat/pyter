#include "cpyter.h"

string *string_init() {
    string *retptr = (string *)malloc(sizeof(string));
    return retptr;
}

void string_print_nnl(string *strval) {
    printf("%s", *strval);
}

void string_print(string *strval) {
    string_print_nnl(strval);
    puts("");
}

void string_del(string *val) {
    free(*val);
    free(val);
}

int string_len(string *val) {
    return strlen(*val);
}

string *string_add(string *left, string *right) {
    char **retval = (char **)malloc(sizeof(char *));
    *retval = (char *)malloc(strlen(*left)+strlen(*right));
    strcpy(*retval, *left);
    strcpy(*retval+strlen(*left), *right);
    return retval;
}

int string_eq(string *left, string *right) {
    return !strcmp(*left, *right);
}

string *string_cpy(string *val) {
    char **retptr = (char **)malloc(sizeof(char *));
    *retptr = strdup(*val);
    return retptr;
}

char *string_get(string *val, int ind) {
    return *val+ind;
}

void string_set(string *val, int ind, char *ch) {
    *val[ind] = *ch;
}

string *string_slice(string *val, int start, int stop, int step) {
    char **retptr = (char **)malloc(sizeof(char *));
    if (stop < 0)
        stop += string_len(val);
    if (start < 0)
        start += string_len(val);
    int len = stop - start / step + 1;
    *retptr = (char *)malloc(len+1);
    int i = 0;
    if (step > 0)
        while (start+step*i < stop)
            string_set(retptr, i, string_get(val, start+step*i++));
    else if (step < 0)
        while (start+step*i > stop)
            string_set(retptr, i, string_get(val, start+step*i++));
    char z = 0;
    string_set(retptr, i, &z);
    return retptr;
}

/*-----------------------------------------------------------------------------*/

string *string_frchs(char *chval) {
    string *retptr = string_init();
    *retptr = strdup(chval);
    return retptr;
}

int string_eqchs(string *strval, char *chval) {
    string *rightval = string_frchs(chval);
    int retval = string_eq(strval, rightval);
    free(rightval);
    return retval;
}

char *string_tochs(string *strval) {
    return strdup(*strval);
}