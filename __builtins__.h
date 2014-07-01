#ifndef __BUILTINS__H
#define __BUILTINS__H

typedef struct pyclass pyclass;
typedef struct pymodule pymodule;

#include "pytype/pyclass.h"
#include "pytype/pymodule.h"
#include "pytype/others.h"

pyclass NoneType_class;
pyclass int_class;
pyclass bool_class;
pyclass float_class;
pyclass complex_class;
pyclass str_class;
pyclass list_class;
pyclass tuple_class;
pyclass set_class;
pyclass dict_class;
pyclass generator_class;
pyclass function_class;
pyclass type_class;
pyclass module_class;
pyclass range_class;
pymodule __builtins__module;

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