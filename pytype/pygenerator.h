#include "../environment.h"

typedef struct pygenerator pygenerator;

struct pygenerator {
    int type;
    // pyclass *class;
    void *_suite;
    environment *local_env;
    char *id;
};

pygenerator *pygenerator_init(void *_suite, environment *local_env, char *id);
pygenerator *pygenerator_next(void *);