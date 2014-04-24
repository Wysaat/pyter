#include "../evaluate.h"
#include "../list.h"
#include "../environment.h"

typedef struct pyfunction pyfunction;

struct pyfunction {
    int type;
    identifier *id;
    list *parameters;
    void *fsuite;
    environment *env;
};

void *pyfunction__call__(void *, void *);