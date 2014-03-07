#include "cpyter.h"

int main() {
    // integer *a = INTEGER_NODE();
    // a->value = 500;
    // integer *b = INTEGER_NODE();
    // b->value = 70432;
    // integer *c = integer__add__(a, b);
    // integer__print__(c);
    // int x = 881824;
    // char *y = itoa(x);
    // printf("y is %s\n", y);
    // printf("x is %d\n", x);
    // printf("y is %s\n", y);
    mem_block *blk1 = mem_str("56169432423645234243150165154532423442353626264745846320441");
    mem_block *blk2 = mem_str("415425165165031454251654514314150365156104");
    integer *a = integer__init__(blk1);
    integer *b = integer__init__(blk2);
    integer *c = integer__add__(a, b);
    mem_block *blk3 = integer__str__(c);
    // mem_block *mb1 = integer__str__(a);
    // mem_block *mb2 = integer__str__(b);
    // mem_print(mb1);
    // mem_print(mb2);
    mem_print(blk1);
    mem_print(blk2);
    mem_print(blk3);
    return 0;
}