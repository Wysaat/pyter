#ifndef PYTYPE_PY__BUILTINS__H
#define PYTYPE_PY__BUILTINS__H

#include "pyclass.h"

typedef struct pybuiltin_function pybuiltin_function;

struct pybuiltin_function {
    int type;
    int ref;
    pyclass *class;
    char *id;
    void *(* func)();
    void *bound;
};

pybuiltin_function *pybuiltin_function__init__(char *id, void *func);
void *pybuiltin_function__call__(void *, void *);
void pybuiltin_function_del(void *vptr);
void pybuiltin_function_ref(void *vptr);

#endif /* PYTYPE_PY__BUILTINS__H */