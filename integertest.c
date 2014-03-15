#include "cpyter.h"

int main() {

/* test integer__init__ */
    // mem_block *blk1 = mem_str("0047201474618485759040998705027158154577");
    // mem_block *blk2 = mem_str("95064341434249523426534242342352768547");
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

/* test integer__cpy__ */
    // integer *x = (integer__init__(mem_str("0047201474618485759040998705027158154577")));
    // integer *y = integer__cpy__(x);
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));

/* test add/sub */
    // integer *x = (integer__init__(mem_str("1088676821651466301081527589335665881446")));
    // integer *y = (integer__init__(mem_str("95064341434249523426534242342352768547")));
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));
    // integer *z = integer__add__(x, y);
    // integer *w = integer__sub__(x, y);
    // mem_print(integer__str__(z));
    // mem_print(integer__str__(w));
    // printf("w->index is %d\n", w->index);

/* test mul */
    // integer *x = integer__neg__(integer__init__(mem_str("432523524423525324324222222222225356263252442")));
    // integer *y = integer__init__(mem_str("95064341434249523426534242342352768547"));
    // integer *m = integer__mul__(x, y);
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));
    // mem_print(integer__str__(m));
    // mem_print(integer__str__(integer__node__mul__(x, y)));
    // printf("m->index is %d\n", m->index);

/* test div */
    // #100: 33751618337928430092168819988197264971632

    // integer *x = (integer__init__(mem_str("43252352442352532432422222222222535626332")));
    integer *x = integer__init__(mem_str("43252352442352"));
    integer *y = integer__init__(mem_str("4324444445"));
    // integer *a = integer__init__(mem_str("0028474562208354090892429268364861991716"));
    // integer *b = integer__init__(mem_str("95064341434249523426534242342352768547"));
    // integer *c = integer__sub__(a, b);
    // mem_print(integer__str__(c));
    // exit(0);
    int i;
    for (i = 0; i < 100000; i++) {
        x = integer__sub__(x, y);
        printf("%d:\n", i);
        printf("%p\n", x);
        mem_print(integer__str__(x));
        mem_print(integer__str__(y));
        puts("");
    }
    // integer *m = integer__div__(x, y);
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));
    // mem_print(integer__str__(m));

    // integer *a = integer__mkempt__(13);
    // printf("a->index is %d\n", a->index);
    // mem_print(integer__str__(a));

    // integer *node1 = integer__init__(mem_str("12345"));
    // integer *node2 = integer__init__(mem_str("67890"));
    // mem_print(integer__str__(node1));
    // mem_print(integer__str__(node2));
    // integer *val = integer__node__mul__(node1, node2);
    // integer *val2 = integer__mul__(node1, node2);
    // mem_print(integer__str__(val));
    // mem_print(integer__str__(val2));

    // integer *a = integer__init__(mem_str("15943242"));
    // integer *b = integer__init__(mem_str("534295"));
    // integer *c = integer__or__(a, b);
    // mem_print(integer__str__(a));
    // mem_print(integer__str__(b));
    // mem_print(integer__str__(c));

/* test INTEGER_NODE */
    // integer *zero = INTEGER_NODE();
    // mem_print(integer__str__(zero));

    return 0;
}