#include "../list.h"
#include "pyint.h"
#include "pybool.h"

typedef struct pytuple pytuple;
struct pytuple {
    int type;
    list *values;
};

pytuple *pytuple__init__();
pytuple *pytuple__add__(void *, void *);
pytuple *pytuple__mul__(pytuple *, void *);
pybool *pytuple__eq__(void *, void *);
void *pytuple__getitem__(void *, void *);