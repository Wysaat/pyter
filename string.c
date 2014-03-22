string *string_init() {
    string *retptr = (string *)malloc(sizeof(string));
    return retptr;
}

void string_del(string *val) {
    free(*val);
    free(val);
}

string *string_add(string *left, string *right) {
    char *retval = (char *)malloc(strlen(*left)+strlen(*right));
    strcpy(retval, *left);
    strcpy(retval+strlen(*left), *right);
    return &retval;
}

int string_eq(string *left, string *right) {
    return !strcmp(*left, *right);
}

string *string_cpy(string *val) {
    char **retptr = (char **)malloc(sizeof(char *));
    *retptr = strdup(*val);
    return retptr;
}

char *string_ssci(string *val, int ind) {
    return *val+ind;
}

string *string_slice(string *val, int start, int stop, int step) {
    char **retptr = (char **)malloc(sizeof(char *));
    int len = stop - start / step + 1;
    *retptr = (char *)malloc(len+1);
    int i = 0;
    while (start+step*i < stop)
        string_ssci(retptr, i) = string_ssci(val, start+step*i++);
    string_ssci(retptr, i) = 0;
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