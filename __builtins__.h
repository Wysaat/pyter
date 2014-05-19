#include "environment.h"

pyclass int_class, list_class;

void def_print(environment *);
void def_next(environment *);
void def_int(environment *env);
void def_list(environment *env);