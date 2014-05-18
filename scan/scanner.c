#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

scanner *sc_init(FILE *stream) {
    scanner *retptr = (scanner *)malloc(sizeof(scanner));
    memset(retptr, 0, sizeof(scanner));
    retptr->stream = stream;
    retptr->indentation_stack = list_node();
    int *zero = (int *)malloc(sizeof(int));
    *zero = 0;
    retptr->indentation_stack->content = zero;
    retptr->eolf = 1;
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
    size_t read, len = 0;
    read = getline(&line, &len, sc->stream);
    if (read != -1) {
        sc->line = line;
        sc->ln++;
        sc->ll = read;
        sc->ind = 0;
        sc->eolf = 0;
    }

    int i, indentation = 0;
    /*  not supporting tab now */
    for (i = 0; sc->line[i] == ' '; i++) {
        indentation++;
    }

    list *ptr = sc->indentation_stack;
    while (ptr->next)
        ptr = ptr->next;
    if (*(int *)ptr->content == indentation) {
        sc->indentf = 0;
        sc->dedentf = 0;
    }
    else if (*(int *)ptr->content < indentation) {
        list *newnode = list_node();
        int *intptr = (int *)malloc(sizeof(int));
        *intptr = indentation;
        newnode->content = intptr;
        ptr->next = newnode;
        newnode->prev = ptr;

        sc->indentf = 1;
        sc->dedentf = 0;
    }
    else {
        sc->indentf = 0;
        sc->dedentf = 0;
        while (*(int *)ptr->content > indentation) {
            sc->dedentf++;
            ptr = ptr->prev;
            if (!ptr)
                break;
            free(ptr->next->content);
            free(ptr->next);
            ptr->next = 0;
        }
    }
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