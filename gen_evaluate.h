#ifndef GEN_EVALUATE_H
#define GEN_EVALUATE_H

#include "list.h"
#include "integer.h"
#include "environment.h"
#include "execute.h"

void *identifier_gen_evaluate(identifier *, environment *);
void *int_expr_gen_evaluate(int_expr *);
void *float_expr_gen_evaluate(float_expr *);
void *imag_expr_gen_evaluate(imag_expr *);
void *str_expr_gen_evaluate(str_expr *);
void *parenth_form_gen_evaluate(parenth_form *, environment *);
void *list_expr_gen_evaluate(list_expr *, environment *);
void *set_expr_gen_evaluate(set_expr *, environment *);
void *dict_expr_gen_evaluate(dict_expr *, environment *);
void *list_comprehension_gen_evaluate(list_comprehension *, environment *);
void *yield_atom_gen_evaluate(yield_atom *, environment *);
void *attributeref_gen_evaluate(attributeref *, environment *);
void *slice_expr_gen_evaluate(slice_expr *, environment *);
void *slicing_gen_evaluate(slicing *, environment *);
void *subscription_gen_evaluate(subscription *, environment *);
void *call_gen_evaluate(call *, environment *);
void *power_gen_evaluate(power *, environment *);
void *u_expr_gen_evaluate(u_expr *, environment *);
void *b_expr_gen_evaluate(b_expr *, environment *);
void *not_test_gen_evaluate(not_test *, environment *);
void *comparison_gen_evaluate(comparison *, environment *);
void *conditional_expression_gen_evaluate(conditional_expression *, environment *);
void *expression_list_gen_evaluate(expression_list *, environment *);
void *gen_evaluate(void *, environment *);
void print_nnl(void *);
void print(void *);

parenth_form *parenth_form_copy(parenth_form *expr);
list_expr *list_expr_copy(list_expr *expr);
set_expr *set_expr_copy(set_expr *expr);
dict_expr *dict_expr_copy(dict_expr *expr);
yield_atom *yield_atom_copy(yield_atom *expr);
attributeref *attributeref_copy(attributeref *expr);
void *slice_expr_copy(slice_expr *expr);
void *subsc_expr_copy(subsc_expr *expr);
void *slicing_copy(slicing *expr);
void *subscription_copy(subscription *expr);
void *call_copy(call *ptr);
void *power_copy(power *expr);
void *u_expr_copy(u_expr *expr);
void *b_expr_copy(b_expr *expr);
comparison *comparison_copy(comparison *expr);
void *not_test_copy(not_test *expr);
void *conditional_expression_copy(conditional_expression *expr);
void *expression_list_copy(expression_list *expr);

#endif /* GEN_EVALUATE_H */