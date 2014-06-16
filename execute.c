#include "execute.h"
#include "types.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include "pytype/methods.h"
#include "pytype/pylist.h"
#include "pytype/pyfunction.h"
#include "pytype/pyclass.h"
#include "pytype/others.h"
#include "pytype/pyint.h"
#include "pytype/pybool.h"
#include "cpyter.h"

void *EXPRESSION_STMT(void *expression_list) {
    expression_stmt *retptr = (expression_stmt *)malloc(sizeof(expression_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = expression_stmt_t;
    retptr->expression_list = expression_list;
    return retptr;
}

void *ASSIGNMENT_STMT(list *targets_list, void *expressions) {
    assignment_stmt *retptr = (assignment_stmt *)malloc(sizeof(assignment_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = assignment_stmt_t;
    retptr->targets_list = targets_list;
    retptr->expressions = expressions;
    return retptr;
}

void *RETURN_STMT(void *expressions) {
    return_stmt *retptr = (return_stmt *)malloc(sizeof(return_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = return_stmt_t;
    retptr->expressions = expressions;
    return retptr;
}

void *YIELD_STMT(void *expressions) {
    yield_stmt *retptr = (yield_stmt *)malloc(sizeof(yield_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = yield_stmt_t;
    retptr->expressions = expressions;
    return retptr;
}

void yield_stmt_del(void *vptr) {
    yield_stmt *ptr = (yield_stmt *)vptr;
    del(ptr->expressions);
    free(ptr);
}

void *BREAK_STMT() {
    break_stmt *retptr = (break_stmt *)malloc(sizeof(break_stmt));
    retptr->type = break_stmt_t;
    return retptr;
}

void *CONTINUE_STMT() {
    continue_stmt *retptr = (continue_stmt *)malloc(sizeof(continue_stmt));
    retptr->type = continue_stmt_t;
    return retptr;
}

void *PASS_STMT() {
    pass_stmt *retptr = (pass_stmt *)malloc(sizeof(pass_stmt));
    retptr->type = pass_stmt_t;
    return retptr;
}

void *IMPORT_STMT(char *module_name) {
    import_stmt *retptr = (import_stmt *)malloc(sizeof(import_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = import_stmt_t;
    retptr->module_name = module_name;
    return retptr;
}

void *DEL_STMT(void *expressions) {
    del_stmt *retptr = (del_stmt *)malloc(sizeof(del_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = del_stmt_t;
    if (type(expressions) == expression_list_t)
        retptr->target_list = ((expression_list *)expressions)->expr_head;
    else {
        retptr->target_list = list_node();
        list_append_content(retptr->target_list, expressions);
    }
    return retptr;
}

void *STMT_LIST(list *stmts) {
    stmt_list *retptr = (stmt_list *)malloc(sizeof(stmt_list));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = stmt_list_t;
    retptr->stmts = stmts;
    return retptr;
}

void *IF_STMT(list *condition_list, list *suite_list) {
    if_stmt *retptr = (if_stmt *)malloc(sizeof(if_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = if_stmt_t;
    retptr->condition_list = condition_list;
    retptr->suite_list = suite_list;
    return retptr;
}

void *WHILE_STMT(void *condition, list *suite_list) {
    while_stmt *retptr = (while_stmt *)malloc(sizeof(while_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = while_stmt_t;
    retptr->condition = condition;
    retptr->suite_list = suite_list;
    return retptr;
}

void *FOR_STMT(void *targets, void *expressions, list *suite_list) {
    for_stmt *retptr = (for_stmt *)malloc(sizeof(for_stmt));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = for_stmt_t;
    retptr->targets = targets;
    retptr->expressions = expressions;
    retptr->suite_list = suite_list;
    return retptr;
}

void *FUNCDEF(identifier *id, list *parameters, void *fsuite, int yield, 
                  expression_list *assign_target_list, expression_list *assign_expr_list) {
    funcdef *retptr = (funcdef *)malloc(sizeof(funcdef));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = funcdef_t;
    retptr->id = id;
    retptr->parameters = parameters;
    retptr->fsuite = fsuite;
    retptr->yield = yield;
    retptr->assign_target_list = assign_target_list;
    retptr->assign_expr_list = assign_expr_list;
    return retptr;
}

void *CLASSDEF(identifier *id, list *inheritance, void *_suite) {
    classdef *retptr = (classdef *)malloc(sizeof(classdef));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = classdef_t;
    retptr->id = id;
    retptr->inheritance = inheritance;
    retptr->_suite = _suite;
    return retptr;
}

void *SUITE(list *stmts) {
    suite *retptr = (suite *)malloc(sizeof(suite));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = suite_t;
    retptr->stmts = stmts;
    return retptr;
}

void expression_stmtExecute(void *structure, environment *env, int pf) {
    expression_stmt *stmt = (expression_stmt *)structure;
    void *ptr = evaluate(stmt->expression_list, env);
    if (pf && type(ptr)!=pyNone_t)
        print(ptr);
    del(ptr);
}

void expression_stmt_del(void *vptr) {
    expression_stmt *ptr = (expression_stmt *)vptr;
    del(ptr->expression_list);
    free(ptr);
}

void assignment_stmtExecute(void *structure, environment *env, int pf) {
    assignment_stmt *stmt = (assignment_stmt *)structure;
    void *values = evaluate(stmt->expressions, env);
    list *ptr;
    // a = b = c = 3, a, b and c consist the targets_list
    for (ptr = stmt->targets_list; ptr; ptr = ptr->next)
        store(env, ptr->content, values);
    del(values);
}

void assignment_stmt_del(void *vptr) {
    assignment_stmt *ptr = (assignment_stmt *)vptr;
    del(ptr->expressions);
    free(ptr);
}

void return_stmtExecute(void *structure, environment *env, int pf) {
    return_stmt *stmt = (return_stmt *)structure;
    env->ret = evaluate(stmt->expressions, env);
}

void return_stmt_del(void *vptr) {
    return_stmt *ptr = (return_stmt *)vptr;
    del(ptr->expressions);
    free(ptr);
}

void break_stmtExecute(void *structure, environment *env, int pf) {
    env->_break = 1;
}

void break_stmt_del(void *vptr) {
    free(vptr);
}

void continue_stmtExecute(void *structure, environment *env, int pf) {
    env->_continue = 1;
}

void continue_stmt_del(void *vptr) {
    free(vptr);
}

void pass_stmtExecute(void *structure, environment *env, int pf) {
}

void pass_stmt_del(void *vptr) {
    free(vptr);
}

void import_stmtExecute(void *structure, environment *env, int pf) {
    import_stmt *stmt = (import_stmt *)structure;
    char *filename = stradd(stmt->module_name, ".py");
    FILE *stream = fopen(filename, "r");
    pymodule *mptr = pymodule_init(stmt->module_name);
    interpret(stream, mptr->env);
    store(env, IDENTIFIER(mptr->name), mptr);
}

void import_stmt_del(void *vptr) {
    import_stmt *ptr = (import_stmt *)vptr;
    free(ptr->module_name);
    free(ptr);
}

void del_stmtExecute(void *structure, environment *env, int pf) {
    del_stmt *stmt = (del_stmt *)structure;
    list *ptr, *ptr2;
    for (ptr = stmt->target_list; ptr; ptr = ptr->next) {
        if (type(ptr->content) == identifier_t) {
            for (ptr2 = env->val_dict; ptr2; ptr2 = ptr2->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr2->content;
                if (!strcmp(entry->id, ((identifier *)ptr->content)->value)) {
                    del(entry->value);
                    if (ptr2->prev)
                        ptr2->prev->next = ptr2->next;
                    if (ptr2->next)
                        ptr2->next->prev = ptr2->prev;
                    free(ptr2->content);
                    free(ptr2);
                }
            }
        }
    }
}

void del_stmt_del(void *vptr) {
    del_stmt *ptr = (del_stmt *)vptr;
    del(ptr->target_list);
    free(ptr);
}

void stmt_listExecute(void *structure, environment *env, int pf) {
    stmt_list *stmt = (stmt_list *)structure;
    list *ptr;
    for (ptr = stmt->stmts; ptr; ptr = ptr->next) {
        execute(ptr->content, env, pf);
    }
}

void stmt_list_del(void *vptr) {
    stmt_list *ptr = (stmt_list *)vptr;
    del(ptr->stmts);
    free(ptr);
}

void if_stmtExecute(void *structure, environment *env, int pf) {
    if_stmt *stmt= (if_stmt *)structure;
    list *ptr1, *ptr2;
    for (ptr1 = stmt->condition_list, ptr2 = stmt->suite_list;
             ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
        void *temp1 = evaluate(ptr1->content, env);
        pybool *temp2 = __bool__(temp1);
        if (is_true(temp2)) {
            execute(ptr2->content, env, pf);
            del(temp2);
            del(temp1);
            return;
        }
        del(temp2);
        del(temp1);
    }
    if (ptr2)
        execute(ptr2->content, env, pf);
}

void if_stmt_del(void *vptr) {
    if_stmt *ptr = (if_stmt *)vptr;
    del(ptr->condition_list);
    del(ptr->suite_list);
    free(ptr);
}

void while_stmtExecute(void *structure, environment *env, int pf) {
    while_stmt *stmt = (while_stmt *)structure;
    void *cond = evaluate(stmt->condition, env);
    pybool *truth = __bool__(cond);
    while (is_true(truth) && !env->ret) {
        del(truth);
        del(cond);
        execute(stmt->suite_list->content, env, pf);
        if (env->_break)
            break;
        if (env->_continue)
            env->_continue = 0;
        cond = evaluate(stmt->condition, env);
        truth = __bool__(cond);
    }
    if (env->_break) {
        env->_break = 0;
        return;
    }
    else {
        del(truth);
        del(cond);
    }
    if (stmt->suite_list->next)
        execute(stmt->suite_list->next->content, env, pf);
}

void while_stmt_del(void *vptr) {
    while_stmt *ptr = (while_stmt *)vptr;
    del(ptr->condition);
    del(ptr->suite_list);
    free(ptr);
}

void for_stmtExecute(void *structure, environment *env, int pf) {
    for_stmt *stmt = (for_stmt *)structure;
    void *values_list = evaluate(stmt->expressions, env);
    if (type(values_list) == pylist_t ||
        type(values_list) == pytuple_t ||
        type(values_list) == pyset_t) {
        list *ptr;
        for (ptr = ((pylist *)values_list)->values; ptr; ptr = ptr->next) {
            store(env, stmt->targets, ptr->content);
            execute(stmt->suite_list->content, env, pf);
            if (env->_break)
                break;
            if (env->_continue)
                env->_continue = 0;
        }
        if (env->_break) {
            env->_break = 0;
            return;
        }
        if (stmt->suite_list->next)
            execute(stmt->suite_list->next->content, env, pf);
    }

    else if (type(values_list) == pyrange_t) {
        pyrange *range = (pyrange *)values_list;
        pyint *index = int_to_pyint(0);
        while (1) {
            pyint *intval = __getitem__(range, index);
            if (is_true(pyint__ge__(intval, range->stop)))
                break;
            store(env, stmt->targets, intval);
            execute(stmt->suite_list->content, env, pf);
            if (env->_break)
                break;
            if (env->_continue)
                env->_continue = 0;
            pyint__inc__(index);
        }
        if (env->_break) {
            env->_break = 0;
            return;
        }
        if (stmt->suite_list->next)
            execute(stmt->suite_list->next->content, env, pf);
    }
}

void for_stmt_del(void *vptr) {
    for_stmt *ptr = (for_stmt *)vptr;
    del(ptr->targets);
    del(ptr->expressions);
    del(ptr->suite_list);
    free(ptr);
}

void funcdefExecute(void *structure, environment *env, int pf) {
    funcdef *stmt = (funcdef *)structure;
    pyfunction *func = (pyfunction *)malloc(sizeof(pyfunction));
    memset(func, 0, sizeof(pyfunction));
    func->type = pyfunction_t;
    func->id = stmt->id;
    func->parameters = stmt->parameters;
    func->fsuite = stmt->fsuite;
    func->bound = 0;
    func->yield = stmt->yield;
    func->env = env;
    func->assign_target_list = stmt->assign_target_list;
    if (stmt->assign_target_list)
        func->assign_values = evaluate(stmt->assign_expr_list, env);
    func->ref = 0;

    store(env, stmt->id, func);
}

void funcdef_del(void *vptr) {
    funcdef *ptr = (funcdef *)vptr;
    free(ptr);
}

void classdefExecute(void *structure, environment *env, int pf) {
    classdef *stmt = (classdef *)structure;
    pyclass *class = pyclass__init__(strdup(stmt->id->value));
    if (stmt->inheritance) {
        class->inheritance = list_node();
        list *ptr;
        for (ptr = stmt->inheritance; ptr; ptr = ptr->next) {
            list_append_content(class->inheritance, evaluate(ptr->content, env));
        }
    }
    class->env->outer = env;
    execute(stmt->_suite, class->env, 0);
    class->env->outer = 0;
    store(env, stmt->id, class);
}

void classdef_del(void *vptr) {
    classdef *ptr = (classdef *)vptr;
    del(ptr->id);
    if (ptr->inheritance)
        del(ptr->inheritance);
    del(ptr->_suite);
    free(ptr);
}

void suiteExecute(void *structure, environment *env, int pf) {
    suite *stmt = (suite *)structure;
    list *ptr;
    for (ptr = stmt->stmts; ptr; ptr = ptr->next)
        execute(ptr->content, env, pf);
}

void suite_del(void *vptr) {
    suite *ptr = (suite *)vptr;
    del(ptr->stmts);
    free(ptr);
}

/*
 * CAUTION: break!!! NEVER forget to BREAK!!!
 * hours have been wasted !!!
 */
void execute(void *structure, environment *env, int pf) {
    if (env->ret || env->_break || env->_continue)
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
        case break_stmt_t:
            break_stmtExecute(structure, env, pf);
            break;
        case continue_stmt_t:
            continue_stmtExecute(structure, env, pf);
            break;
        case pass_stmt_t:
            pass_stmtExecute(structure, env, pf);
            break;
        case import_stmt_t:
            import_stmtExecute(structure, env, pf);
            break;
        case del_stmt_t:
            del_stmtExecute(structure, env, pf);
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
        case classdef_t:
            classdefExecute(structure, env, pf);
            break;
        case suite_t:
            suiteExecute(structure, env, pf);
            break;
    }
}
