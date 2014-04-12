#ifndef CPYTER_H
#define CPYTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "evaluate.h"
#include "execute.h"

#include "types.h"
#include "string.h"
#include "integer.h"
#include "list.h"

#include "scan/buffer.h"
#include "scan/scanner.h"
#include "scan/token.h"

#include "pytype/methods.h"
#include "pytype/pyint.h"
#include "pytype/pyfloat.h"
#include "pytype/pycomplex.h"
#include "pytype/pystr.h"
#include "pytype/pybool.h"
#include "pytype/pylist.h"
#include "pytype/pytuple.h"
#include "pytype/pyset.h"
#include "pytype/pydict.h"

#include "environment.h"

int is_alph(char );
int is_num(char );
int is_alphnum(char );

void *parse_u_expr(scanner *sc);
void *parse_expression(scanner *sc);
list *pa_exprs(scanner *, char *);
list **pa_dict_items(scanner *);
void *pa_sll_or_subs(scanner *);

char *itoa(int );
char *lltoa(long long );

#endif /* CPYTER_H */