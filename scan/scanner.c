#include "../cpyter.h"

struct scanner {
    string *line;
    int ll; /* line length */
    int ind; /* index in a line */
    int ln;
    int mlf; /* multi lines flag */
    int rbf; /* rollback flag */
    int eolf;
    string *lasttk;
};

scanner *sc_init() {
    scanner *retptr = (scanner *)malloc(sizeof(scanner));
    memset(retptr, 0, sizeof(scanner));
    return retptr;
}

char sc_curch(scanner *sc) {
    return sc->line[sc->ind];
}

char sc_nxtch(scanner *sc) {
    return sc->line[sc->ind+1];
}

char *sc_curchs(scanner *sc, int len) {
    return strndup(sc->line+sc->ind, len);
}

void sc_inci(scanner *sc) {
    sc->ind++;
}

char sc_readch(scanner *sc) {
    sc_inci(sc);
    return sc->line[sc->ind-1];
}

char sc_readchs(scanner *sc, int len) {
    char *ret = sc_curchs(sc, len);
    sc->ind += strlen(ret);
    return ret;
}

string *sc_lasttk(scanner *sc) {
    return sc->lasttk;
}

void sc_tkstore(scanner *sc, string *token) {
    string_del(sc->lasttk);
    sc->lasttk = token;
}

int sc_eolf_get(scanner *sc) {
    return sc->eolf;
}

void sc_eolf_set(scanner *sc) {
    sc->eolf = 1;
}

void sc_eolf_clr(scanner *sc) {
    sc->eolf = 0;
}

int sc_mlf_get(scanner *sc) {
    return sc->rbf;
}

int sc_rbf_get(scanner *sc) {
    return sc->rbf;
}

void sc_rbf_set(scanner *sc) {
    sc->rbf = 1;
}

void sc_rbf_clr(scanner *sc) {
    sc->rbf = 0;
}

void sc_getline(scanner *sc, FILE *stream) {
    if (stream == stdin)
        printf(">>> ");
    char *line = 0;
    int read, len = 0;
    int read = geline(&line, &len, stdin);
    if (read != -1) {
        sc->line = line;
        sc->ln++;
        sc->ll = read;
        sc.ind = 0;
        sc_eolf_clr(sc);
    }
}