#include "execute.h"
#include "evaluate.h"
#include "gen_execute.h"
#include "gen_evaluate.h"
#include "types.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include "pytype/methods.h"
#include "pytype/pylist.h"
#include "pytype/pyfunction.h"
#include "pytype/pyclass.h"

expression_stmt *expression_stmt_copy(expression_stmt *stmt) {
    void *expression_list = copy(stmt->expression_list);
    return EXPRESSION_STMT(expression_list);
}

assignment_stmt *assignment_stmt_copy(assignment_stmt *stmt) {
    void *targets = copy(stmt->targets);
    void *expressions = copy(stmt->expressions);
    return ASSIGNMENT_STMT(targets, expressions);
}

yield_stmt *yield_stmt_copy(yield_stmt *stmt) {
    void *expressions = copy(stmt->expressions);
    return YIELD_STMT(expressions);
}

stmt_list *stmt_list_copy(stmt_list *stmt) {
    stmt_list *retptr = (stmt_list *)malloc(sizeof(stmt_list));
    retptr->type = stmt_list_t;
    retptr->stmts = copy(stmt->stmts);
    retptr->ptr = 0;
    return retptr;
}

if_stmt *if_stmt_copy(if_stmt *stmt) {
    if_stmt *retptr = (if_stmt *)malloc(sizeof(if_stmt));
    retptr->type = if_stmt_t;
    retptr->condition_list = copy(stmt->condition_list);
    retptr->suite_list = copy(stmt->suite_list);
    retptr->ptr1 = 0;
    retptr->ptr2 = 0;
    return retptr;
}

while_stmt *while_stmt_copy(while_stmt *stmt) {
    while_stmt *retptr = (while_stmt *)malloc(sizeof(while_stmt));
    retptr->type = while_stmt_t;
    retptr->condition = copy(stmt->condition);
    retptr->suite_list = copy(stmt->suite_list);
    retptr->cond_val = 0;
}

for_stmt *for_stmt_copy(for_stmt *stmt) {
    for_stmt *retptr = (for_stmt *)malloc(sizeof(for_stmt));
    retptr->type = for_stmt_t;
    retptr->targets = copy(stmt->targets);
    retptr->expressions = copy(stmt->expressions);
    retptr->suite_list = copy(stmt->suite_list);
    retptr->values_list = 0;
    retptr->list_ptr = 0;
    return retptr;
}

suite *suite_copy(suite *stmt) {
    suite *retptr = (suite *)malloc(sizeof(suite));
    retptr->type = suite_t;
    retptr->stmts = copy(stmt->stmts);
    retptr->ptr = 0;
    return retptr;
}

void expression_stmt_gen_execute(void *structure, environment *env, int pf) {
    expression_stmt *stmt = (expression_stmt *)structure;
    void *ptr = gen_evaluate(stmt->expression_list, env);
    if (env->yield)
        return;
    if (pf)
        print(ptr);
}

void assignment_stmt_gen_execute(void *structure, environment *env, int pf) {
    assignment_stmt *stmt = (assignment_stmt *)structure;
    void *targets = stmt->targets;
    void *values = gen_evaluate(stmt->expressions, env);
    if (env->yield)
        return;
    store(env, targets, values);
}

void return_stmt_gen_execute(void *structure, environment *env, int pf) {
    return_stmt *stmt = (return_stmt *)structure;
    env->ret = gen_evaluate(stmt->expressions, env);
}

void yield_stmt_gen_execute(void *structure, environment *env, int pf) {
    yield_stmt *stmt = (yield_stmt *)structure;
    if (stmt->yielded)
        stmt->yielded = 0;
    else {
        env->yield = gen_evaluate(stmt->expressions, env);
        stmt->yielded = 1;
    }
}

void stmt_list_gen_execute(void *structure, environment *env, int pf) {
    stmt_list *stmt = (stmt_list *)structure;
    if (!stmt->ptr)
        stmt->ptr = stmt->stmts;
    for ( ; stmt->ptr; stmt->ptr = stmt->ptr->next) {
        gen_execute(stmt->ptr->content, env, pf);
        if (env->yield)
            return;
    }
}

void if_stmt_gen_execute(void *structure, environment *env, int pf) {
    if_stmt *stmt= (if_stmt *)structure;
    list *ptr;

    if (!stmt->ptr2) {
        stmt->ptr1 = stmt->condition_list;
        stmt->ptr2 = stmt->suite_list;
    }
    for ( ; stmt->ptr1; stmt->ptr1 = stmt->ptr1->next, stmt->ptr2 = stmt->ptr2->next) {
        void *val = gen_evaluate(stmt->ptr1->content, env);
        if (env->yield)
            return;
        if (is_true(__bool__(val))) {
            gen_execute(stmt->ptr2->content, env, pf);
            if (env->yield)
                return;
            if (type(stmt->ptr2->content) == suite_t)
                ptr = ((suite *)(stmt->ptr2->content))->ptr;
            else if (type(stmt->ptr2->content) == stmt_list_t)
                ptr = ((stmt_list *)(stmt->ptr2->content))->ptr;
            if (!ptr) {
                stmt->ptr1 = 0;
                stmt->ptr2 = 0;
            }
            return;
        }
    }
    if (stmt->ptr2) {
        gen_execute(stmt->ptr2->content, env, pf);
        if (env->yield)
            return;
        if (type(stmt->ptr2->content) == suite_t)
            ptr = ((suite *)(stmt->ptr2->content))->ptr;
        else if (type(stmt->ptr2->content) == stmt_list_t)
            ptr = ((stmt_list *)(stmt->ptr2->content))->ptr;
        if (!ptr)
            stmt->ptr2 = 0;
    }
}

void while_stmt_gen_execute(void *structure, environment *env, int pf) {
    while_stmt *stmt = (while_stmt *)structure;

    if (!stmt->cond_val) {
        void *vptr = gen_evaluate(stmt->condition, env);
        if (env->yield)
            return;
        stmt->cond_val =vptr;
    }
    pybool *truth = __bool__(stmt->cond_val);
    while (is_true(truth) && !env->yield) {
        gen_execute(stmt->suite_list->content, env, pf);
        if (env->yield)
            return;
        if (env->_break)
            break;
        if (env->_continue)
            env->_continue = 0;
        stmt->cond_val = gen_evaluate(stmt->condition, env);
        if (env->yield)
            return;
        truth = __bool__(stmt->cond_val);
    }
    if (env->_break) {
        env->_break = 0;
        return;
    }
    if (stmt->suite_list->next)
        gen_execute(stmt->suite_list->next->content, env, pf);
}

void for_stmt_gen_execute(void *structure, environment *env, int pf) {
    for_stmt *stmt = (for_stmt *)structure;

    if (!stmt->values_list && !stmt->index) {
        void *values_list = gen_evaluate(stmt->expressions, env);
        if (env->yield)
            return;
        if (type(values_list) == pylist_t || type(values_list) == pytuple_t || type(values_list) == pyset_t)
            stmt->values_list = values_list;
        else if (type(values_list) == pyrange_t)
            stmt->range = values_list;
    }

    if (stmt->values_list) {
        list *ptr;
        if (!stmt->list_ptr)
            stmt->list_ptr = ((pylist *)stmt->values_list)->values;
        for ( ; stmt->list_ptr; stmt->list_ptr = stmt->list_ptr->next) {
            store(env, stmt->targets, stmt->list_ptr->content);
            gen_execute(stmt->suite_list->content, env, pf);
            if (env->yield)
                return;
            if (env->_break)
                break;
            if (env->_continue)
                env->_continue = 0;
        }
        if (env->_break) {
            env->_break = 0;
            stmt->list_ptr = 0;
            stmt->values_list = 0;
            return;
        }
        if (stmt->suite_list->next) {
            gen_execute(stmt->suite_list->next->content, env, pf);
            if (env->yield)
                return;
        }
        stmt->list_ptr = 0;
        stmt->values_list = 0;
    }

    else if (stmt->range) {
        if (!stmt->index)
            stmt->index = int_to_pyint(0);
        while (1) {
            pyint *intval = __getitem__(stmt->range, stmt->index);
            if (is_true(pyint__ge__(intval, stmt->range->stop)))
                break;
            store(env, stmt->targets, intval);
            gen_execute(stmt->suite_list->content, env, pf);
            if (env->yield)
                return;
            if (env->_break)
                break;
            if (env->_continue)
                env->_continue = 0;
            pyint__inc__(stmt->index);
        }
        if (env->_break) {
            env->_break = 0;
            stmt->index = 0;
            return;
        }
        if (stmt->suite_list->next) {
            gen_execute(stmt->suite_list->next->content, env, pf);
            if (env->yield)
                return;
        }
        stmt->index = 0;
    }
}

void suite_gen_execute(void *structure, environment *env, int pf) {
    suite *stmt = (suite *)structure;
    if (!stmt->ptr)
        stmt->ptr = stmt->stmts;
    for ( ; stmt->ptr; stmt->ptr = stmt->ptr->next) {
        gen_execute(stmt->ptr->content, env, pf);
        if (env->yield)
            return;
    }
}

/*
 * CAUTION: break!!! NEVER forget BREAK!!!
 * hours have been wasted !!!
 */
void gen_execute(void *structure, environment *env, int pf) {
    if (env->ret || env->_break || env->_continue)
        return;
    switch (type(structure)) {
        case expression_stmt_t:
            expression_stmt_gen_execute(structure, env, pf);
            break;
        case assignment_stmt_t:
            assignment_stmt_gen_execute(structure, env, pf);
            break;
        case return_stmt_t:
            return_stmt_gen_execute(structure, env, pf);
            break;
        case yield_stmt_t:
            yield_stmt_gen_execute(structure, env, pf);
            break;
        case break_stmt_t:
            break_stmtExecute(structure, env, pf);
            break;
        case continue_stmt_t:
            continue_stmtExecute(structure, env, pf);
            break;
        case pass_stmt_t:
            pass_stmtExecute(structure, env, pf);
        case stmt_list_t:
            stmt_list_gen_execute(structure, env, pf);
            break;
        case if_stmt_t:
            if_stmt_gen_execute(structure, env, pf);
            break;
        case while_stmt_t:
            while_stmt_gen_execute(structure, env, pf);
            break;
        case for_stmt_t:
            for_stmt_gen_execute(structure, env, pf);
            break;
        case funcdef_t:
            funcdefExecute(structure, env, pf);
            break;
        case classdef_t:
            classdefExecute(structure, env, pf);
            break;
        case suite_t:
            suite_gen_execute(structure, env, pf);
            break;
    }
}

list *list_copy(list *head) {
    list *retptr = list_node();
    if (list_is_empty(head))
        return retptr;
    list *curptr = retptr;
    list *ptr = head;
    while (ptr) {
        curptr->content = copy(ptr->content);
        ptr = ptr->next;
        curptr->next = list_node();
        curptr->next->prev = curptr;
        curptr = curptr->next;
    }
    curptr->prev->next = 0;
    free(curptr);
    return retptr;
}

void *copy(void *ptr) {
    switch (type(ptr)) {
        case list_t:
            return list_copy((list *)ptr);

        case expression_stmt_t:
            return expression_stmt_copy((expression_stmt *)ptr);
        case assignment_stmt_t:
            return assignment_stmt_copy((assignment_stmt *)ptr);
        case return_stmt_t:
            return ptr;
        case yield_stmt_t:
            return yield_stmt_copy((yield_stmt *)ptr);
        case break_stmt_t:
            return ptr;
        case continue_stmt_t:
            return ptr;
        case pass_stmt_t:
            return ptr;
        case stmt_list_t:
            return stmt_list_copy((stmt_list *)ptr);
        case if_stmt_t:
            return if_stmt_copy((if_stmt *)ptr);
        case while_stmt_t:
            return while_stmt_copy((while_stmt *)ptr);
        case for_stmt_t:
            return for_stmt_copy((for_stmt *)ptr);
        case funcdef_t:
            return ptr;
        case classdef_t:
            return ptr;
        case suite_t:
            return suite_copy((suite *)ptr);

        case identifier_t:
            return ptr;
        case int_expr_t:
            return ptr;
        case float_expr_t:
            return ptr;
        case imag_expr_t:
            return ptr;
        case str_expr_t:
            return ptr;
        case parenth_form_t:
            return parenth_form_copy((parenth_form *)ptr);
        case list_expr_t:
            return list_expr_copy((list_expr *)ptr);
        case list_comprehension_t:
            return ptr;
        case set_expr_t:
            return set_expr_copy((set_expr *)ptr);
        case dict_expr_t:
            return dict_expr_copy((dict_expr *)ptr);
        case yield_atom_t:
            return yield_atom_copy((yield_atom *)ptr);
        case attributeref_t:
            return attributeref_copy((attributeref *)ptr);
        case slice_expr_t:
            return slice_expr_copy((slice_expr *)ptr);
        case slicing_t:
            return slicing_copy((slicing *)ptr);
        case subscription_t:
            return subscription_copy((subscription *)ptr);
        case call_t:
            return call_copy((call *)ptr);
        case power_t:
            return power_copy((power *)ptr);
        case u_expr_t:
            return u_expr_copy((u_expr *)ptr);
        case b_expr_t:
            return b_expr_copy((b_expr *)ptr);
        case comparison_t:
            return comparison_copy((comparison *)ptr);
        case not_test_t:
            return not_test_copy((not_test *)ptr);
        case conditional_expression_t:
            return conditional_expression_copy((conditional_expression *)ptr);
        case lambda_expr_t:
            return ptr;
        case expression_list_t:
            return expression_list_copy((expression_list *)ptr);
    }
}