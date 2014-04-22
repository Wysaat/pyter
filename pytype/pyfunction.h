#include "../evaluate.h"
#include "../list.h"

typedef struct pyfunction pyfunction;

struct pyfunction {
    int type;
    identifier *id;
    list *parameters;
    void *fsuite;
};

void *pyfunction__call__(void *, void *);