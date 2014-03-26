#include "../string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *a = "hello, world";
    char *b = stradd(a, ", again");
    char *c = strslice(a, 2, 8, 1);
    char *d = strslice(a, 5, -2, 4);
    char *e = strslice(a, 10, 1, 3);

    puts(a);
    puts(b);
    puts(c);
    puts(d);
    puts(e);

    return 0;
}