#include <string.h>
#include "token.h"

int is_spctk(char *token) {
    int len = sizeof(spctks) / sizeof(*spctks), i;
    for (i = 0; i < len; i++)
        if (!strcmp(token, spctks[i]))
            return 1;
    return 0;
}

int is_strprfx(char *token) {
    int len = sizeof(strprfxes) / sizeof(*strprfxes), i;
    for (i = 0; i < len; i++)
        if (!strcmp(token, strprfxes[i]))
            return 1;
    return 0;
}

int is_cmpop(char *token) {
    int len = sizeof(cmpops) / sizeof(*cmpops), i;
    for (i = 0; i < len; i++)
        if (!strcmp(token, cmpops[i]))
            return 1;
    return 0;
}

int is_int(char *token) {
    int i;
    for (i = 0; i < strlen(token); i++)
        if (token[i] < '0' || token[i] > '9')
            return 0;
    return 1;
}

int is_str(char *token) {
    if (token[0] == '"' || token[0] == '\'')
        return 1;
    return 0;
}