#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

scanner *sc_init(FILE *stream) {
    scanner *retptr = (scanner *)malloc(sizeof(scanner));
    memset(retptr, 0, sizeof(scanner));
    retptr->stream = stream;
    return retptr;
}

char sc_curch(scanner *sc) {
    return sc->line[sc->ind];
}

char sc_nxtch(scanner *sc) {
    return sc->line[sc->ind+1];
}

char *sc_curchs(scanner *sc, int len) {
    char *retval = (char *)malloc(len+1);
    strncpy(retval, sc->line+sc->ind, len);
    retval[len] = 0;
    return retval;
}

char sc_readch(scanner *sc) {
    sc->ind++;
    return sc->line[sc->ind-1];
}

char *sc_readchs(scanner *sc, int len) {
    char *retval = sc_curchs(sc, len);
    sc->ind += len;
    return retval;
}

void sc_getline(scanner *sc) {
    if (sc->stream == stdin)
        printf(">>> ");
    char *line = 0;
    int read, len = 0;
    read = getline(&line, &len, sc->stream);
    if (read != -1) {
        sc->line = line;
        sc->ln++;
        sc->ll = read;
        sc->ind = 0;
        sc->eolf = 0;
    }
}

int sc_indent(scanner *sc) {
}

void sc_dump(scanner *sc) {
    printf("-----begin of scanner dump---------------------------\n");
    printf("scanner->stream is %p\n", sc->stream);
    printf("scanner->line is %s", sc->line);
    printf("scanner->ll is %d\n", sc->ll);
    printf("scanner->ind is %d\n", sc->ind);
    printf("scanner->ln is %d\n", sc->ln);
    printf("scanner->mlf is %d\n", sc->mlf);
    printf("scanner->rbf is %d\n", sc->rbf);
    printf("scanner->eolf is %d\n", sc->eolf);
    printf("scanner->lasttk is %s\n", sc->lasttk);
    printf("-----end of scanner dump-----------------------------\n");
}