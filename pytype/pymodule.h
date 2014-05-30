#include "../environment.h"

typedef struct pymodule pymodule;

struct pymodule {
    int type;
    char *name;
    environment *env;
};

pymodule *pymodule_init(char *name);