#ifndef SCAN_SCANNER_H
#define SCAN_SCANNER_H

#include <stdio.h>
#include "../list.h"

typedef struct scanner scanner;

struct scanner {
    FILE *stream;
    char *line;
    int ll;  /* line length */
    int ind;
    int ln;  /* line number */
    int mlf; /* multi lines flag */
    int rbf; /* rollback flag */
    int eolf;
    char *lasttk; /* last token */
    list *indentation_stack;
    int indentf;
    int dedentf;
    int yield;  /* for yield */
};

scanner *sc_init(FILE *stream);
char sc_curch(scanner *sc);
char sc_nxtch(scanner *sc);
char *sc_curchs(scanner *sc, int len);
void sc_inci(scanner *sc);
char sc_readch(scanner *sc);
char *sc_readchs(scanner *sc, int len);
void sc_getline(scanner *sc);
void sc_dump(scanner *sc);

#endif /* SCAN_SCANNER_H */