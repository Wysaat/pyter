/* "<>" is deprecated in Python 3 */
char spctks[][4] = { "+", "-", "*", "**", "/", "//", "%",
                        "<<", ">>", "&", "|", "^", "~",
                        "<", ">", "<=", ">=", "==", "!=", "<>",
                        "(", ")", "[", "]", "{", "}", "@",
                        ",", ":", "`", "=", ";",
                        "+=", "-=", "*=", "/=", "//=", "%=",
                        "&=", "|=", "^=", ">>=", "<<=", "**=",

                        "!", "$", "?", "#", "\\",

                        "+=", "-=", "*=", "/=", "//=", "%=", "**=",
                        ">>=", "<<=", "&=", "^=", "|=", };

char strprfxes[][2] = { "r", "u", "R", "U", };

char cmpops[][3] = { "<", ">", "==", ">=", "<=", "<>", "!=", };

typedef struct scanner scanner;

struct scanner {
    string *line;
    int ll;  /* line length */
    int ind;
    int ln;  /* line number */
    int mlf; /* multi lines flag */
    int rbf; /* rollback flag */
    string *lasttk; /* last token */
};