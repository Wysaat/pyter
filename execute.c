#include "execute.h"
#include "types.h"
#include "list.h"
#include <stdlib.h>
#include "pytype/methods.h"

void *EXPRESSION_STMT(void *expression_list) {
    expression_stmt *retptr = (expression_stmt *)malloc(sizeof(expression_stmt));
    retptr->type = expression_stmt_t;
    retptr->expression_list = expression_list;
    return retptr;
}

void *ASSIGNMENT_STMT(void *targets, void *expressions) {
    assignment_stmt *retptr = (assignment_stmt *)malloc(sizeof(assignment_stmt));
    retptr->type = assignment_stmt_t;
    if (type(targets) == expression_list_t) {
        retptr->targets = targets;
        retptr->expressions = expressions;
        return retptr;
    }
    else {
        list *list1 = list_node();
        list *list2 = list_node();
        list1->content = targets;
        list2->content = expressions;
        expression_list *_targets = EXPRESSION_LIST(list1);
        expression_list *_expressions = EXPRESSION_LIST(list2);
        retptr->targets = _targets;
        retptr->expressions = _expressions;
        return retptr;
    }
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

void *SUITE(list *stmts) {
    suite *retptr = (suite *)malloc(sizeof(suite));
    retptr->type = suite_t;
    retptr->stmts = stmts;
    return retptr;
}

void *expression_stmtExecute(void *structure, environment *env) {
    expression_stmt *stmt = (expression_stmt *)structure;
    return evaluate(stmt->expression_list, env);
}

void *assignment_stmtExecute(void *structure, environment *env) {
    assignment_stmt *stmt = (assignment_stmt *)structure;
    list *targets = stmt->targets->expr_head;
    list *expressions = stmt->expressions->expr_head;
    list *ptr1, *ptr2;
    for (ptr1 = targets, ptr2 = expressions; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next)
        store(env, ptr1->content, evaluate(ptr2->content, env));
    return 0;
}

void *stmt_listExecute(void *structure, environment *env) {
    stmt_list *stmt = (stmt_list *)structure;
    list *ptr;
    list *retlist = list_node();
    for (ptr = stmt->stmts; ptr; ptr = ptr->next)
        list_append_content(retlist, execute(ptr->content, env));
    return retlist;
}

void *if_stmtExecute(void *structure, environment *env) {
    if_stmt *stmt= (if_stmt *)structure;
    list *ptr1, *ptr2;
    for (ptr1 = stmt->condition_list, ptr2 = stmt->suite_list;
             ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        /* MEMORY LEAKAGE */
        if (is_true(__bool__(evaluate(ptr1->content, env))))
            return execute(ptr2->content, env);
    }
    if (ptr2)
        return execute(ptr2->content, env);
    else
        return 0;
}

void *suiteExecute(void *structure, environment *env) {
    suite *stmt = (suite *)structure;
    list *retlist = list_node(), *ptr;
    for (ptr = stmt->stmts; ptr; ptr = ptr->next)
        list_append_content(retlist, execute(ptr->content, env));
    return retlist;
}

void *execute(void *structure, environment *env) {
    switch (type(structure)) {
        case expression_stmt_t:
            return expression_stmtExecute(structure, env);
        case assignment_stmt_t:
            return assignment_stmtExecute(structure, env);
        case stmt_list_t:
            return stmt_listExecute(structure, env);
        case if_stmt_t:
            return if_stmtExecute(structure, env);
        case suite_t:
            return suiteExecute(structure, env);
    }
}