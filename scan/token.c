#include "../cpyter.h"

int is_spctk(string *token) {
    int len = sizeof(spctks) / sizeof(*spctks), i;
    for (i = 0; i < len; i++)
        if (string_eq(token, &spctks[i]))
            return 1;
    return 0;
}

int is_strprfx(string *token) {
    int len = sizeof(strprfxes) / sizeof(*strprfxes), i;
    for (i = 0; i < len; i++)
        if (string_eq(token, &strprfxes[i]))
            return 1;
    return 0;
}

int is_cmpop(string *token) {
    int len = sizeof(cmpops) / sizeof(*cmpops), i;
    for (i = 0; i < len; i++)
        if (string_eq(token, &cmpops[i]))
            return 1;
    return 0;
}

string *tk_init(buffer *buff) {
    char **retptr;
    *retptr = (char *)malloc(buff_len(buff)+1);
    int offs = 0;
    buffer *ptr = buff;
    while (ptr) {
        strcpy(retptr+offs, ptr->value);
        offs += strlen(ptr->value);
    }
    return retptr;
}