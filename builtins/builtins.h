#include "../pytype/pystr.h"
#include "../pytype/others.h"

/* str.c */
pystr *str_init(pyargument *argument);

/* int.c */
pyint *int_init(pyargument *argument);

/* functions.c */
void *_abs(pyargument *argument);
void *_len(pyargument *argument);
void *_next(pyargument *argument);
void *_print(pyargument *argument);
