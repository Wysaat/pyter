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
#include "pytype/pyfunction.h"
#include "pytype/pyclass.h"
#include "pytype/others.h"

#include "environment.h"

#include "__builtins__.h"

int is_alph(char );
int is_num(char );
int is_alphnum(char );

void *parse_u_expr(scanner *sc);
void *parse_conditional_expression(scanner *sc);
void *parse_lambda_expr(scanner *sc);
void *parse_lambda_expr_nocond(scanner *sc);
void *parse_expression(scanner *sc);
void *parse_expression_nocond(scanner *sc);
list *pa_exprs(scanner *, char *);
list **pa_dict_items(scanner *);
void *pa_sll_or_subs(scanner *);
void *parse_expression_list(scanner *sc, char *ending);
void *parse_target_list(scanner *sc, char *ending);
list *parse_argument_list(scanner *sc, char *ending);
void *parse_comp_for(scanner *sc, void *expression);
void *parse_comp_if(scanner *sc, void *expression);
void *parse_comp_iter(scanner *sc, void *expression);
void parse_comp_for_2(scanner *sc, list *comp_iters);
void parse_comp_if_2(scanner *sc, list *comp_iters);
void *parse_simple_stmt(scanner *);
void *parse_stmt_list(scanner *);
void *parse_suite(scanner *);
void *parse_if_stmt(scanner *);
void *parse_compound_stmt(scanner *);
void *parse_stmt(scanner *);

char *itoa(int );
char *lltoa(long long );

#endif /* CPYTER_H */