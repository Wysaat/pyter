#ifndef PYTYPE_PY__BUILTINS__H
#define PYTYPE_PY__BUILTINS__H

typedef struct pybuiltin_function pybuiltin_function;

struct pybuiltin_function {
    int type;
    char *id;
    void *(* func)();
    void *bound;
};

pybuiltin_function *pybuiltin_function__init__(char *id, void *func);
void *pybuiltin_function__call__(void *, void *);

#endif /* PYTYPE_PY__BUILTINS__H */