#ifndef EXECUTE_H
#define EXECUTE_H

#include "evaluate.h"
#include "environment.h"
#include "pytype/pytuple.h"
#include "pytype/others.h"

typedef struct identifier identifier;
typedef struct environment environment;
typedef struct expression_list expression_list;

typedef struct expression_stmt {
    int type;
    void *expression_list;
} expression_stmt;

typedef struct assignment_stmt {
    int type;
    list *targets_list;
    void *expressions;
} assignment_stmt;

typedef struct return_stmt {
    int type;
    void *expressions;
} return_stmt;

typedef struct yield_stmt {
    int type;
    void *expressions;
    int yielded;
} yield_stmt;

typedef struct break_stmt {
    int type;
} break_stmt;

typedef struct continue_stmt {
    int type;
} continue_stmt;

typedef struct pass_stmt {
    int type;
} pass_stmt;

typedef struct import_stmt {
    int type;
    list *module_name_list;
    list *as_name_list;
} import_stmt;

typedef struct import_stmt2 {
    int type;
    char *module_name;
    list *var_name_list;
    list *as_name_list;
} import_stmt2;

typedef struct del_stmt {
    int type;
    list *target_list;
} del_stmt;

typedef struct stmt_list {
    int type;
    list *stmts;
    list *ptr;  /* for yield */
} stmt_list;

typedef struct if_stmt {
    int type;
    list *condition_list;
    list *suite_list;
    list *ptr1, *ptr2;  /* for yield */
} if_stmt;

typedef struct while_stmt {
    int type;
    void *condition;
    list *suite_list;
    void *cond_val;  /* for yield */
} while_stmt;

typedef struct for_stmt {
    int type;
    void *targets;
    void *expressions;
    list *suite_list;
    void *values_list; /* for yield */
    list *list_ptr;    /* for yield */
    pyrange *range;    /* for range in yield */
    pyint *index;      /* for range in yield */
} for_stmt;

typedef struct funcdef {
    int type;
    identifier *id;
    list *parameters;
    void *fsuite;
    int yield;
    expression_list *assign_target_list;  // can be 0
    expression_list *assign_expr_list;  // can be 0
} funcdef;

typedef struct classdef {
    int type;
    identifier *id;
    list *inheritance;  // can be 0
    void *_suite;
} classdef;

typedef struct suite {
    int type;
    list *stmts;
    list *ptr;  /* for yield */
} suite;

void *EXPRESSION_STMT(void *);
void *ASSIGNMENT_STMT(list *, void *);
void *RETURN_STMT(void *);
void *YIELD_STMT(void *);
void *BREAK_STMT();
void *CONTINUE_STMT();
void *PASS_STMT();
void *IMPORT_STMT(list *, list *);
void *IMPORT_STMT2(char *, list *, list *);
void *DEL_STMT(void *);
void *STMT_LIST(list *);
void *IF_STMT(list *condition_list, list *suite_list);
void *WHILE_STMT(void *condition, list *suite_list);
void *FOR_STMT(void *targets, void *expressions, list *suite_list);
void *FUNCDEF(identifier *id, list *parameters, void *fsuite, int yield, expression_list *assign_target_list, expression_list *assign_expr_list);
void *CLASSDEF(identifier *id, list *inheritance, void *_suite);
void *SUITE(list *stmts);
void expression_stmtExecute(void *structure, environment *env, int pf);
void assignment_stmtExecute(void *structure, environment *env, int pf);
void return_stmtExecute(void *structure, environment *env, int pf);
void yield_stmtExecute(void *structure, environment *env, int pf);
void break_stmtExecute(void *structure, environment *env, int pf);
void continue_stmtExecute(void *structure, environment *env, int pf);
void pass_stmtExecute(void *structure, environment *env, int pf);
void import_stmtExecute(void *structure, environment *env, int pf);
void del_stmtExecute(void *structure, environment *env, int pf);
void stmt_listExecute(void *structure, environment *env, int pf);
void if_stmtExecute(void *structure, environment *env, int pf);
void while_stmtExecute(void *structure, environment *env, int pf);
void for_stmtExecute(void *structure, environment *env, int pf);
void funcdefExecute(void *structure, environment *env, int pf);
void classdefExecute(void *structure, environment *env, int pf);
void suiteExecute(void *structure, environment *env, int pf);
void execute(void *structure, environment *env, int pf);

void expression_stmt_del(void *vptr);
void assignment_stmt_del(void *vptr);
void return_stmt_del(void *vptr);
void yield_stmt_del(void *vptr);
void break_stmt_del(void *vptr);
void continue_stmt_del(void *vptr);
void pass_stmt_del(void *vptr);
void import_stmt_del(void *vptr);
void del_stmt_del(void *vptr);
void stmt_list_del(void *vptr);
void if_stmt_del(void *vptr);
void while_stmt_del(void *vptr);
void for_stmt_del(void *vptr);
void funcdef_del(void *vptr);
void classdef_del(void *vptr);
void suite_del(void *vptr);

#endif /* EXECUTE_H */