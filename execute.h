#include "evaluate.h"
#include "environment.h"

typedef struct expression_stmt {
    int type;
    void *expression_list;
} expression_stmt;

typedef struct assignment_stmt {
    int type;
    void *targets;
    void *expressions;
} assignment_stmt;

typedef struct stmt_list {
    int type;
    list *stmts;
} stmt_list;

typedef struct if_stmt {
    int type;
    list *condition_list;
    list *suite_list;
} if_stmt;

typedef struct while_stmt {
    int type;
    void *condition;
    list *suite_list;
} while_stmt;

typedef struct for_stmt {
    int type;
    void *targets;
    void *expressions;
    list *suite_list;
} for_stmt;

typedef struct suite {
    int type;
    list *stmts;
} suite;

void *EXPRESSION_STMT(void *);
void *ASSIGNMENT_STMT(void *, void *);
void *STMT_LIST(list *);
void *IF_STMT(list *condition_list, list *suite_list);
void *WHILE_STMT(void *condition, list *suite_list);
void *FOR_STMT(void *targets, void *expressions, list *suite_list);
void *SUITE(list *stmts);
void expression_stmtExecute(void *structure, environment *env, int pf);
void assignment_stmtExecute(void *structure, environment *env, int pf);
void stmt_listExecute(void *structure, environment *env, int pf);
void if_stmtExecute(void *structure, environment *env, int pf);
void while_stmtExecute(void *structure, environment *env, int pf);
void suiteExecute(void *structure, environment *env, int pf);
void execute(void *structure, environment *env, int pf);