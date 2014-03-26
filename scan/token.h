#ifndef SCAN_TOKEN_H
#define SCAN_TOKEN_H

int is_spctk(char *token);
int is_strprfx(char *token);
int is_cmpop(char *token);
int is_int(char *token);
int is_str(char *token);

/* "<>" is deprecated in Python 3 */
static char spctks[][4] = { "+", "-", "*", "**", "/", "//", "%",
                        "<<", ">>", "&", "|", "^", "~",
                        "<", ">", "<=", ">=", "==", "!=", "<>",
                        "(", ")", "[", "]", "{", "}", "@",
                        ",", ":", "`", "=", ";",
                        "+=", "-=", "*=", "/=", "//=", "%=",
                        "&=", "|=", "^=", ">>=", "<<=", "**=",

                        "!", "$", "?", "#", "\\",

                        "+=", "-=", "*=", "/=", "//=", "%=", "**=",
                        ">>=", "<<=", "&=", "^=", "|=", };

static char strprfxes[][2] = { "r", "u", "R", "U", };

static char cmpops[][3] = { "<", ">", "==", ">=", "<=", "<>", "!=", };

#endif /* SCAN_TOKEN_H */