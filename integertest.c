#include "cpyter.h"

int main() {
    // integer *a = integer__init__(mem_str("8888888"));
    // integer *b = integer__init__(mem_str("8888888"));
    // integer *c = integer__sub__(a, b);
    // mem_print(integer__str__(c));
    // exit(0);

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
    // integer *a = integer__init__(mem_str("93141431203248796775676198794378705994"));
    // integer *b = integer__init__(mem_str("95064341434249523426534242342352768547"));
    //                                   // "95064341434249523326534242332352768547"
    // integer *c = integer__sub__(a, b);
    // mem_print(integer__str__(a));
    // mem_print(integer__str__(b));
    // mem_print(integer__str__(c));
    // exit(0);

    integer *x = integer__init__(mem_str("7096278976412553801576094515275238891792118306389298398476332583863723162649973008036929605274858882"));
    integer *x_n = integer__neg__(x);
    integer *z = integer__init__(mem_str("7096278976412553801576094515275238891792118306389298398476332583863723162649973008036929605274858882"));
    integer *y = integer__init__(mem_str("58318072537305140708872100995438527734141988105602703115250249521103186142739037"));
    integer *w = integer__init__(mem_str("583180725373051407088721009954385277341419881056027031152502495211031861427390374324243213424324"));
    // integer *a = integer__init__(mem_str("0028474562208354090892429268364861991716"));
    // integer *b = integer__init__(mem_str("95064341434249523426534242342352768547"));
    // integer *c = integer__sub__(a, b);
    // mem_print(integer__str__(c));
    // exit(0);
    // int i;
    // int end = 10000;
    // for (i = 0; i < end; i++) {
    //     x = integer__sub__(x, y);
    //     // printf("%d:\n", i);
    //     // // printf("%p\n", x);
    //     // mem_print(integer__str__(x));
    //     // mem_print(integer__str__(y));
    //     // puts("");
    //     // if (i == 349) while (1) ;
    //     // if (i == end-1) while (1) ;
    // }
    // mem_print(integer__str__(x));
    integer *r = integer__init__(mem_str("8"));
    integer *r_n = integer__neg__(r);
    integer *s = integer__init__(mem_str("2"));
    integer *m = integer__div__(r_n, s);
    // mem_print(integer__str__(x));
    // mem_print(integer__str__(y));
    mem_print(integer__str__(m));

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

    // integer *head = INTEGER_NODE();
    // head->sign = '-';
    // head->value = -10;
    // head->index = 8;
    // mem_print(integer__str__(head));
    // return 0;
}