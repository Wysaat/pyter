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

scanner *sc_init();
char sc_curch(scanner *sc);
char sc_nxtch(scanner *sc);
char *sc_curchs(scanner *sc, int len);
void sc_inci(scanner *sc);
char sc_readch(scanner *sc);
char sc_readchs(scanner *sc, int len);
string *sc_lasttk(scanner *sc);
void sc_tkstore(scanner *sc, string *token);
int sc_eolf_get(scanner *sc);
void sc_eolf_set(scanner *sc);
void sc_eolf_clr(scanner *sc);
int sc_mlf_get(scanner *sc);
int sc_rbf_get(scanner *sc);
void sc_rbf_set(scanner *sc);
void sc_rbf_clr(scanner *sc);
void sc_getline(scanner *sc, FILE *stream);