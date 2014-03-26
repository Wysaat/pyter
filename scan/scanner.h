#ifndef SCAN_SCANNER_H
#define SCAN_SCANNER_H

#include <stdio.h>

typedef struct scanner scanner;

struct scanner {
    char *line;
    int ll;  /* line length */
    int ind;
    int ln;  /* line number */
    int mlf; /* multi lines flag */
    int rbf; /* rollback flag */
    int eolf;
    char *lasttk; /* last token */
};

scanner *sc_init();
char sc_curch(scanner *sc);
char sc_nxtch(scanner *sc);
char *sc_curchs(scanner *sc, int len);
void sc_inci(scanner *sc);
char sc_readch(scanner *sc);
char *sc_readchs(scanner *sc, int len);
void sc_getline(scanner *sc, FILE *stream);
void sc_dump(scanner *sc);

#endif /* SCAN_SCANNER_H */