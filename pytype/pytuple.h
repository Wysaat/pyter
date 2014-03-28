#include "../list.h"
#include "pyint.h"

typedef struct pytuple pytuple;
struct pytuple {
    int type;
    list *values;
};

pytuple *pytuple__init__();
pytuple *pytuple__add__(pytuple *, pytuple *);
pytuple *pytuple__mul__(pytuple *, void *);