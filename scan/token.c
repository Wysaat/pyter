#include <string.h>
#include "token.h"
#include "../cpyter.h"

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

int is_identifier(char *token) {
    if (!is_alph(token[0]) && token[0] != '_')
        return 0;
    if (strlen(token) == 1)
        return 1;
    int i;
    for (i = 1; i < strlen(token); i++)
        if (!is_alphnum(token[i]) && token[i] != '_')
            return 0;
    return 1;
}

int is_int(char *token) {
    int i;
    for (i = 0; i < strlen(token); i++)
        if (token[i] < '0' || token[i] > '9')
            return 0;
    return 1;
}

int is_float(char *token) {
    if (strlen(token) < 2)
        return 0;
    if (token[strlen(token)-1] == 'j' ||
        token[strlen(token)-1] == 'J')
        return 0;
    int i;
    for (i = 0; i < strlen(token); i++)
        if (token[i] == '.')
            return 1;
    return 0;
}

int is_imag(char *token) {
    return (strlen(token) > 1 && !is_alph(token[0]) &&
            (token[strlen(token)-1] == 'j' || token[strlen(token)-1] == 'J'));
}

int is_str(char *token) {
    if (token[0] == '"' || token[0] == '\'')
        return 1;
    return 0;
}
