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
    integer *x = integer__neg__(integer__init__(mem_str("24325")));
    integer *y = integer__neg__(integer__init__(mem_str("84235234423")));
    mem_block *blk1 = mem_str("56169432423645234243150165154532423442353626264745846320441");
    mem_block *blk2 = mem_str("415425165165031454251654514314150365156104");
    integer *a = integer__init__(blk1);
    integer *b = integer__init__(blk2);
    integer *c = integer__add__(a, b);
    integer *d = integer__sub__(a, b);
    integer *e = integer__sub__(b, a);
    mem_block *blk3 = integer__str__(c);
    mem_block *blk4 = integer__str__(d);
    mem_block *blk5 = integer__str__(e);
    // mem_block *mb1 = integer__str__(a);
    // mem_block *mb2 = integer__str__(b);
    // mem_print(mb1);
    // mem_print(mb2);
    mem_print(blk1);
    mem_print(blk2);
    mem_print(blk3);
    mem_print(blk4);
    mem_print(blk5);
    mem_print(integer__str__(x));
    mem_print(integer__str__(y));
    integer *z = integer__add__(x, y);
    integer *w = integer__sub__(x, y);
    mem_print(integer__str__(z));
    mem_print(integer__str__(w));
    printf("a->index is %d\n", a->index);
    return 0;
}