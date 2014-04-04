#include "pybool.h"

typedef struct pystr pystr;
struct pystr {
    int type;
    char *value;
};

pystr *pystr__init__();
void pystr__del__(pystr *);
pystr *pystr__mul__(void *, void *);
pystr *pystr__add__(void *, void *);
pybool *pystr__eq__(void *, void *);

void pystr__print__(pystr *);
void pystr__puts__(pystr *);