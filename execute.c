#include "execute.h"
#include "types.h"
#include "list.h"
#include <stdlib.h>
#include "pytype/methods.h"
#include "pytype/pylist.h"
#include "pytype/pyfunction.h"

void *EXPRESSION_STMT(void *expression_list) {
    expression_stmt *retptr = (expression_stmt *)malloc(sizeof(expression_stmt));
    retptr->type = expression_stmt_t;
    retptr->expression_list = expression_list;
    return retptr;
}

void *ASSIGNMENT_STMT(void *targets, void *expressions) {
    assignment_stmt *retptr = (assignment_stmt *)malloc(sizeof(assignment_stmt));
    retptr->type = assignment_stmt_t;
    retptr->targets = targets;
    retptr->expressions = expressions;
    return retptr;
}

void *RETURN_STMT(void *expressions) {
    return_stmt *retptr = (return_stmt *)malloc(sizeof(return_stmt));
    retptr->type = return_stmt_t;
    retptr->expressions = expressions;
    return retptr;
}

void *STMT_LIST(list *stmts) {
    stmt_list *retptr=  (stmt_list *)malloc(sizeof(stmt_list));
    retptr->type = stmt_list_t;
    retptr->stmts = stmts;
    return retptr;
}

void *IF_STMT(list *condition_list, list *suite_list) {
    if_stmt *retptr = (if_stmt *)malloc(sizeof(if_stmt));
    retptr->type = if_stmt_t;
    retptr->condition_list = condition_list;
    retptr->suite_list = suite_list;
    return retptr;
}

void *WHILE_STMT(void *condition, list *suite_list) {
    while_stmt *retptr = (while_stmt *)malloc(sizeof(while_stmt));
    retptr->type = while_stmt_t;
    retptr->condition = condition;
    retptr->suite_list = suite_list;
    return retptr;
}

void *FOR_STMT(void *targets, void *expressions, list *suite_list) {
    for_stmt *retptr = (for_stmt *)malloc(sizeof(for_stmt));
    retptr->type = for_stmt_t;
    retptr->targets = targets;
    retptr->expressions = expressions;
    retptr->suite_list = suite_list;
    return retptr;
}

void *FUNCDEF(identifier *id, list *parameters, void *fsuite) {
    funcdef *retptr = (funcdef *)malloc(sizeof(funcdef));
    retptr->type = funcdef_t;
    retptr->id = id;
    retptr->parameters = parameters;
    retptr->fsuite = fsuite;
    return retptr;
}

void *SUITE(list *stmts) {
    suite *retptr = (suite *)malloc(sizeof(suite));
    retptr->type = suite_t;
    retptr->stmts = stmts;
    return retptr;
}

void expression_stmtExecute(void *structure, environment *env, int pf) {
    expression_stmt *stmt = (expression_stmt *)structure;
    void *ptr = evaluate(stmt->expression_list, env);
    if (pf)
        print(ptr);
}

void assignment_stmtExecute(void *structure, environment *env, int pf) {
    assignment_stmt *stmt = (assignment_stmt *)structure;
    void *targets = stmt->targets;
    void *values = evaluate(stmt->expressions, env);
    store(env, targets, values);
}

void return_stmtExecute(void *structure, environment *env, int pf) {
    return_stmt *stmt = (return_stmt *)structure;
    env->ret = evaluate(stmt->expressions, env);
}

void stmt_listExecute(void *structure, environment *env, int pf) {
    stmt_list *stmt = (stmt_list *)structure;
    list *ptr;
    for (ptr = stmt->stmts; ptr; ptr = ptr->next) {
        execute(ptr->content, env, pf);
    }
}

void if_stmtExecute(void *structure, environment *env, int pf) {
    if_stmt *stmt= (if_stmt *)structure;
    list *ptr1, *ptr2;
    for (ptr1 = stmt->condition_list, ptr2 = stmt->suite_list;
             ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        /* MEMORY LEAKAGE */
        if (is_true(__bool__(evaluate(ptr1->content, env)))) {
            execute(ptr2->content, env, pf);
            return;
        }
    }
    if (ptr2)
        execute(ptr2->content, env, pf);
}

void while_stmtExecute(void *structure, environment *env, int pf) {
    while_stmt *stmt = (while_stmt *)structure;
    void *cond = evaluate(stmt->condition, env);
    pybool *truth = __bool__(cond);
    while (is_true(truth)) {
        execute(stmt->suite_list->content, env, pf);
        // __del__(cond);
        // __del__(truth);
        cond = evaluate(stmt->condition, env);
        truth = __bool__(cond);
    }
    // __del__(cond);
    // __del__(truth);
    if (stmt->suite_list->next)
        execute(stmt->suite_list->next->content, env, pf);
}

void for_stmtExecute(void *structure, environment *env, int pf) {
    for_stmt *stmt = (for_stmt *)structure;
    /* MEMORY LEAKAGE: values_list needs __del__ when leaving. */
    void *values_list = evaluate(stmt->expressions, env);
    if (type(values_list) == pylist_t ||
        type(values_list) == pytuple_t ||
        type(values_list) == pyset_t) {
        list *ptr;
        for (ptr = ((pylist *)values_list)->values; ptr; ptr = ptr->next) {
            store(env, stmt->targets, ptr->content);
            execute(stmt->suite_list->content, env, pf);
        }
        if (stmt->suite_list->next)
            execute(stmt->suite_list->next->content, env, pf);
    }
}

void funcdefExecute(void *structure, environment *env, int pf) {
    funcdef *stmt = (funcdef *)structure;
    pyfunction *func = (pyfunction *)malloc(sizeof(pyfunction));
    func->type = pyfunction_t;
    func->id = stmt->id;
    func->parameters = stmt->parameters;
    func->fsuite = stmt->fsuite;
    func->env = env;
    store(env, stmt->id, func);
}

void suiteExecute(void *structure, environment *env, int pf) {
    suite *stmt = (suite *)structure;
    list *ptr;
    for (ptr = stmt->stmts; ptr; ptr = ptr->next)
        execute(ptr->content, env, pf);
}

/*
 * CAUTION: break!!! NEVER forget BREAK!!!
 * hours have been wasted !!!
 */
void execute(void *structure, environment *env, int pf) {
    if (env->ret)
        return;
    switch (type(structure)) {
        case expression_stmt_t:
            expression_stmtExecute(structure, env, pf);
            break;
        case assignment_stmt_t:
            assignment_stmtExecute(structure, env, pf);
            break;
        case return_stmt_t:
            return_stmtExecute(structure, env, pf);
            break;
        case stmt_list_t:
            stmt_listExecute(structure, env, pf);
            break;
        case if_stmt_t:
            if_stmtExecute(structure, env, pf);
            break;
        case while_stmt_t:
            while_stmtExecute(structure, env, pf);
            break;
        case for_stmt_t:
            for_stmtExecute(structure, env, pf);
            break;
        case funcdef_t:
            funcdefExecute(structure, env, pf);
            break;
        case suite_t:
            suiteExecute(structure, env, pf);
            break;
    }
}