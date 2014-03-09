#include "cpyter.h"

int main() {
    integer *x = integer__neg__(integer__init__(mem_str("24325")));
    integer *y = integer__neg__(integer__init__(mem_str("84235234423")));
    // mem_block *blk1 = mem_str("56169432423645234243150165154532423442353626264745846320441");
    // mem_block *blk2 = mem_str("415425165165031454251654514314150365156104");
    // integer *a = integer__init__(blk1);
    // integer *b = integer__init__(blk2);
    // integer *c = integer__add__(a, b);
    // integer *d = integer__sub__(a, b);
    // integer *e = integer__sub__(b, a);
    // mem_block *blk3 = integer__str__(c);
    // mem_block *blk4 = integer__str__(d);
    // mem_block *blk5 = integer__str__(e);
    // mem_print(blk1);
    // mem_print(blk2);
    // mem_print(blk3);
    // mem_print(blk4);
    // mem_print(blk5);
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));
    // integer *z = integer__add__(x, y);
    // integer *w = integer__sub__(x, y);
    // mem_print(integer__str__(z));
    // mem_print(integer__str__(w));
    // printf("w->index is %d\n", w->index);
    integer *m = integer__mul__(x, y);
    mem_print(integer__str__(m));
    printf("m->index is %d\n", m->index);

    // integer *node1 = integer__init__(mem_str("12345"));
    // integer *node2 = integer__init__(mem_str("67890"));
    // mem_print(integer__str__(node1));
    // mem_print(integer__str__(node2));
    // integer *val = integer__node__mul__(node1, node2);
    // integer *val2 = integer__mul__(node1, node2);
    // mem_print(integer__str__(val));
    // mem_print(integer__str__(val2));
    return 0;
}