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
    int eoff;
    char *skip;
    char *ps, *ps1, *ps2; /* interpreter prompt strings */
    int skip_newlines;
    int in_suite;
    list *lines;
    list *line_ptr;
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
int sc_skip(scanner *sc, char ch);

#endif /* SCAN_SCANNER_H */