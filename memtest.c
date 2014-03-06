#include "cpyter.h"

int main() {
    mem_block *blk = mem_str("Hi, therefroM");
    char *a = mem_get(blk, 8);
    printf("a is %s\n", a);
    mem_block *s = mem_slice(blk, 8, 15);
    printf("s is:\n");
    mem_print(s);
    mem_block *another = mem_str("where have you been, lord thomas?");
    mem_insert(another, blk, 8, 0, mem_size(blk));
    mem_print(another);
    printf("mem_size(another) is %d\n", mem_size(another));
    mem_block *test = mem_str("a123456789b123456789c123456789d123456789e123456789");
    mem_print(test);
    printf("mem_size(test) is %d\n", mem_size(test));
    mem_delete(test, 13, 29);
    printf("mem_size(test) is %d\n", mem_size(test));
    mem_print(test);
}