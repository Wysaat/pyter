#include "../pytype/pystr.h"
#include "../pytype/pylist.h"
#include "../pytype/pymodule.h"
#include "../pytype/others.h"

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

/* int.c */
pyint *int_init(pyargument *argument);

/* bool.c */
pybool *bool_init(pyargument *argument);

/* float.c */
pyfloat *float_init(pyargument *argument);

/* str.c */
pystr *str_init(pyargument *argument);

/* list.c */
pylist *list_init(pyargument *argument);

/* tuple.c */
pybool *tuple__gt__(pyargument *argument);
pybool *tuple__lt__(pyargument *argument);
pybool *tuple__eq__(pyargument *argument);

/* functions.c */
void *_abs(pyargument *argument);
void *_len(pyargument *argument);
void *_next(pyargument *argument);
void *_print(pyargument *argument);

/* functions.c,  methods */
void *__mod__(pyargument *argument);
void *__gt__(pyargument *argument);
void *__lt__(pyargument *argument);
void *__eq__(pyargument *argument);

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
void def___builtins__(environment *);

/* everything */
void def_builtins(environment *env);
