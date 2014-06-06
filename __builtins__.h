#include "environment.h"

pyclass int_class;
pyclass list_class;
pyclass range_class;

/* functions */
void def_print(environment *);
void def_next(environment *);
void def_len(environment *env);

/* classes */
void def_int(environment *env);
void def_list(environment *env);
void def_range(environment *env);