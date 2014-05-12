#ifndef GEN_EXECUTE_H
#define GEN_EXECUTE_H

#include "evaluate.h"
#include "environment.h"

void expression_stmt_gen_execute(void *structure, environment *env, int pf);
void assignment_stmt_gen_execute(void *structure, environment *env, int pf);
void return_stmt_gen_execute(void *structure, environment *env, int pf);
void yield_stmt_gen_execute(void *structure, environment *env, int pf);
void stmt_list_gen_execute(void *structure, environment *env, int pf);
void if_stmt_gen_execute(void *structure, environment *env, int pf);
void while_stmt_gen_execute(void *structure, environment *env, int pf);
void for_stmt_gen_execute(void *structure, environment *env, int pf);
void funcdef_gen_execute(void *structure, environment *env, int pf);
void classdef_gen_execute(void *structure, environment *env, int pf);
void suite_gen_execute(void *structure, environment *env, int pf);
void gen_execute(void *structure, environment *env, int pf);

expression_stmt *expression_stmt_copy(expression_stmt *stmt);
assignment_stmt *assignment_stmt_copy(assignment_stmt *stmt);
yield_stmt *yield_stmt_copy(yield_stmt *stmt);
stmt_list *stmt_list_copy(stmt_list *stmt);
if_stmt *if_stmt_copy(if_stmt *stmt);
while_stmt *while_stmt_copy(while_stmt *stmt);
for_stmt *for_stmt_copy(for_stmt *stmt);
suite *suite_copy(suite *stmt);
list *list_copy(list *head);
void *copy(void *ptr);

#endif /* GEN_EXECUTE_H */