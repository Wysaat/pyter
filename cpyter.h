#ifndef CPYTER_H
#define CPYTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
#include "pytype/pystr.h"
#include "pytype/pybool.h"
#include "pytype/pylist.h"
#include "pytype/pytuple.h"
#include "pytype/pyset.h"
#include "pytype/pydict.h"

int is_alph(char );
int is_num(char );
int is_alphnum(char );

void *int_exprEvaluate(int_expr *);
void *str_exprEvaluate(str_expr *);
void *parenth_formEvaluate(parenth_form *);
void *list_exprEvaluate(list_expr *);
void *set_exprEvaluate(set_expr *);
void *dict_exprEvaluate(dict_expr *);
void *powerEvaluate(power *);
void *u_exprEvaluate(u_expr *);
void *b_exprEvaluate(b_expr *);
void *not_testEvaluate(not_test *);
void *comparisonEvaluate(comparison *);
void *conditional_expressionEvaluate(conditional_expression *);
void *expression_listEvaluate(expression_list *);
void *evaluate(void *);
void print_nnl(void *);
void print(void *);

void *parse_u_expr(scanner *sc);
void *parse_expression(scanner *sc);
list *pa_exprs(scanner *, char *);
list **pa_dict_items(scanner *);
void *pa_sll_or_subs(scanner *);

char *itoa(int );
char *lltoa(long long );

#endif /* CPYTER_H */