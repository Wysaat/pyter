#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpyter.h"

int main() {
    mem_block *blk = mem_str("Hi, therefromhere ,whencedaytoday");
    mem_print(blk);
    printf("mem_size(blk) is %d\n", mem_size(blk));
    mem_set(blk, 10, 'T');
    mem_set(blk, 78, 'W');
    mem_print(blk);
    printf("mem_size(blk) is %d\n", mem_size(blk));
    mem_block *a = mem_head();
    mem_set(a, 3, 'G');
    printf("mem_size(a) is %d\n", mem_size(a));
    char t[15];
    mem_ncpy_out(t, blk, 10, 11);
    printf("t is: %s\n", t);
    int i;
    for (i = 0; i < mem_size(blk); i++)
        printf("%c", mem_subscription(blk, i));
    printf("\n");
    char *str = "That's not the case.";
    mem_block *nope = mem_str(str);
    printf("mem_ncmp(str, nope, strlen(str) is %d\n", mem_ncmp(str, nope, 3, strlen(str)));
    printf("mem_match_str(nope, str) is %d\n", mem_match_str(nope, str));
    return 0;
}