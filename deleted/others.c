    while ((unsigned char)sc_curch(sc) == 27) {
        sc_readch(sc);
        if ((unsigned char)sc_curch(sc) == 91) {
            sc_readch(sc);
            if ((unsigned char)sc_curch(sc) == 65) {
                sc_readch(sc);
                sc->line_ptr = sc->line_ptr->prev;
            }
            else if ((unsigned char)sc_curch(sc) == 66) {
                sc_readch(sc);
                sc->line_ptr = sc->line_ptr->next;
            }
            sc->line = sc->line_ptr->content;
            sc->line[strlen(sc->line)-1] = 0;
            sc->ln++;
            sc->ind = strlen(sc->line);
            printf("%s", sc->line);
        }
    }