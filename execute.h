#include "evaluate.h"
#include "environment.h"

typedef struct expression_stmt {
	int type;
	void *expression_list;
} expression_stmt;

typedef struct assignment_stmt {
    int type;
    expression_list *targets;
    expression_list *expressions;
} assignment_stmt;

typedef struct stmt_list {
	int type;
	list *stmts;
} stmt_list;

void *EXPRESSION_STMT(void *);
void *ASSIGNMENT_STMT(void *, void *);
void *STMT_LIST(list *);
void *expression_stmtExecute(void *structure, environment *env);
void *assignment_stmtExecute(void *structure, environment *env);
void *stmt_listExecute(void *structure, environment *env);
void *execute(void *structure, environment *env);