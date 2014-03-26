#include "../scan/scanner.h"
#include <stdio.h>

int main()
{
	scanner *sc = sc_init();
	sc_getline(sc, stdin);
	sc_dump(sc);
	sc_readchs(sc, 5);
	sc_dump(sc);
	sc_readch(sc);
	sc_dump(sc);

	return 0;
}