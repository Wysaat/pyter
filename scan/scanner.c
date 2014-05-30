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
    retptr->eolf = 0;
    retptr->skip = (char *)malloc(4);
    memset(retptr->skip, 0, 4);
    retptr->skip[0] = ' ';
    retptr->skip[1] = '\t';
    retptr->ps1 = strdup(">>> ");
    retptr->ps2 = strdup("... ");
    retptr->ps = retptr->ps1;
    sc_getline(retptr);
    if (retptr->stream == stdin) {
        while (retptr->ll == 1) {
            if (feof(retptr->stream)) {
                retptr->eoff = 1;
                break;
            }
            sc_getline(retptr);
        }
    }
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
        printf("%s", sc->ps);
    char *line = 0;
    size_t read, len = 0;

    if (sc->stream != stdin) {
        char *ch_ptr;
        int skip_the_line = 1;
        while (1) {
            read = getline(&line, &len, sc->stream);
            if (read == -1) {
                sc->eoff = 1;
                return;
            }
            sc->ln++;
            for (ch_ptr = line; *ch_ptr != '\n' && *ch_ptr != 0; ch_ptr++)
                if (*ch_ptr != ' ' && *ch_ptr != '\t')
                    skip_the_line = 0;
            if (!skip_the_line) {
                sc->line = line;
                sc->ll = read;
                sc->ind = 0;
                sc->eolf = 0;
                break;
            }
        }
    }
    else {
        char *ch_ptr;
        int skip_the_line = 1;
        while (1) {
            read = getline(&line, &len, sc->stream);
            if (read == -1) {
                puts("");
                sc->eoff = 1;
                return;
            }
            sc->ln++;
            if (read == 1) {
                sc->line = line;
                sc->ll = read;
                sc->ind = 0;
                sc->eolf = 0;
                break;
            }
            for (ch_ptr = line; *ch_ptr != '\n'; ch_ptr++)
                if (*ch_ptr != ' ' && *ch_ptr != '\t')
                    skip_the_line = 0;
            if (!skip_the_line) {
                sc->line = line;
                sc->ll = read;
                sc->ind = 0;
                sc->eolf = 0;
                break;
            }
            printf("%s", "... ");
        }
    }

    int i, indentation = 0;
    /* not supporting tabs now */
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
    printf("scanner->skip_newlines is %d\n", sc->skip_newlines);
    printf("scanner->ps is %s\n", sc->ps);
    printf("scanner->ps1 is %s\n", sc->ps1);
    printf("scanner->ps2 is %s\n", sc->ps2);
    printf("-----end of scanner dump-----------------------------\n");
}

int sc_skip(scanner *sc, char ch) {
    char *ptr;
    for (ptr = sc->skip; *ptr; ptr++)
        if (*ptr == ch)
            return 1;
    return 0;
}
