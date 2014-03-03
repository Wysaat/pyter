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
    return 0;
}