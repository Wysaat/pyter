#include "environment.h"

pyclass int_class, list_class;
pyclass range_class;

void def_print(environment *);
void def_next(environment *);
void def_len(environment *env);

void def_int(environment *env);
void def_list(environment *env);
void def_range(environment *env);