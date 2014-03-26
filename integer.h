#ifndef INTEGER_H
#define INTEGER_H

#define INTEGER_SZ 5

typedef struct integer integer;
struct integer {
    int value;
    integer *higher;
    integer *lower;
    char sign;
    int index;
};

integer *INTEGER_NODE();
integer *integer__init__(char *);
void integer__del__(integer *);
integer *integer__cpy__(integer *);
integer *integer__neg__(integer *);
integer *integer__abs__(integer *);
integer *integer__invert__(integer *);
char *integer__str__(integer *);
integer *integer__inc__(integer *);
integer *integer__dec__(integer *);
int integer__eq__(integer *, integer *);
int integer__gt__(integer *, integer *);
int integer__lt__(integer *, integer *);
int integer__ge__(integer *, integer *);
int integer__le__(integer *, integer *);
int integer__cmp__(integer *, integer *);
integer *integer__add__(integer *, integer *);
integer *integer__sub__(integer *, integer *);
integer *integer__mkempt__(int size);
integer *integer__node__mul__(integer *, integer *);
integer *integer__mul__(integer *, integer *);
integer *integer__div__(integer *, integer *);
integer *integer__mod__(integer *, integer *);
integer *integer__pow__(integer *, integer *);
integer *integer__lshift__(integer *, integer *);
integer *integer__rshift__(integer *, integer *);
integer *integer__and__(integer *, integer *);
integer *integer__xor__(integer *, integer *);
integer *integer__or__(integer *, integer *);

#endif /* INTEGER_H */