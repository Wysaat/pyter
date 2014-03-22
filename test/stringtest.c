#include "cpyter.h"
#include "string.h"

int main()
{
    string *a = string__from__char__("Hello, world, it is in the stringtest.c file!");
    string__print__(a);
    string *b = string__cpy__(a);
    int i;
    for (i = 0; i < string__len__(a); i++) {
        char *c = string__subscript__(a, i);
        printf("%c", *c);
    }
    puts("");
    string__del__(a);
    string__print__(b);
    return 0;
}