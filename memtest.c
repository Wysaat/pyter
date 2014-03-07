#include "cpyter.h"

int main() {
    mem_block *blk1 = mem_str("hello, world! to");
    mem_block *blk2 = mem_str("day is Friday!");
    mem_print(blk1);
    mem_print(blk2);
    mem_ncpy(blk1, blk2, mem_size(blk1), 0, mem_size(blk2));
    mem_print(blk1);
    return 0;
}