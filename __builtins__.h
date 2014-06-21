#ifndef __BUILTINS__H
#define __BUILTINS__H

#include "pytype/pymodule.h"
#include "pytype/pyclass.h"
typedef struct pymodule pymodule;

pyclass int_class;
pyclass float_class;
pyclass complex_class;
pyclass str_class;
pyclass list_class;
pyclass range_class;
pyclass type_class;
pymodule __builtins__module;

/* functions */
void def_print(environment *);
void def_next(environment *);
void def_len(environment *env);

/* classes */
void def_int(environment *env);
void def_bool(environment *env);
void def_float(environment *env);
void def_complex(environment *env);
void def_str(environment *env);
void def_list(environment *env);
void def_tuple(environment *env);
void def_range(environment *env);
void def_type(environment *env);

/* modules */
void def__builtins__(environment *);

#endif /* __BUILTINS__H */