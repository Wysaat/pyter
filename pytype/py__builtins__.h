typedef struct pybuiltin_function pybuiltin_function;

struct pybuiltin_function {
    int type;
    char *id;
    void *(* func)();
    void *bound;
};

pybuiltin_function *pybuiltin_function__init__(char *id, void *func);
void *pybuiltin_function__call__(void *, void *);