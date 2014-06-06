#include "environment.h"
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "pytype/pylist.h"
#include "pytype/methods.h"
#include "pytype/pyclass.h"
#include "pytype/pyint.h"
#include "pytype/others.h"
#include "cpyter.h"

environment *environment_init(environment *outer) {
    environment *retptr = (environment *)malloc(sizeof(environment));
    memset(retptr, 0, sizeof(*retptr));
    retptr->type = environment_t;
    retptr->val_dict = list_node();
    retptr->outer = outer;
    return retptr;
}

val_dict_entry *val_dict_entry_init(char *id, void *value) {
    val_dict_entry *retptr = (val_dict_entry *)malloc(sizeof(val_dict_entry));
    retptr->type = val_dict_entry_t;
    retptr->id = id;
    retptr->value = value;
    return retptr;
}

void store(environment *env, void *targets, void *values) {
    if (type(targets) == identifier_t) {
        identifier *id = (identifier *)targets;
        if (list_is_empty(env->val_dict)) {
            list_append_content(env->val_dict, val_dict_entry_init(strdup(id->value), values));
            ref_inc(values);
            return;
        }
        list *ptr;
        if (id->value) {
            for (ptr = env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (entry->id && !strcmp(entry->id, id->value)) {
                    del(entry->value);
                    entry->value = values;
                    ref_inc(values);
                    return;
                }
            }
        }
        else { /* nameless identifer, for comprehensions */
            for (ptr = env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (!entry->id) {
                    del(entry->value);
                    entry->value = values;
                    ref_inc(values);
                    return;
                }
            }
        }
        ref_inc(values);
        list_append_content(env->val_dict, val_dict_entry_init(strdup(id->value), values));
    }
    else if (type(targets) == subscription_t) {
        subscription *subsc = (subscription *)targets;
        __setitem__(evaluate(subsc->primary, env), evaluate(subsc->subsc->value, env), values);
    }
    else if (type(targets) == slicing_t) {
        slicing *slic = (slicing *)targets;
        __setitem__(evaluate(slic->primary, env), evaluate(slic->slice, env), values);
    }
    else if (type(targets) == attributeref_t) {
        attributeref *attribref = (attributeref *)targets;
        void *ptr = evaluate(attribref->primary, env);
        if (type(ptr) == instance_t)
            __setattr__(((instance *)ptr)->class, ptr, PYSTR(attribref->id->value), values);
        else if (type(ptr) == pyclass_t)
            __setattr__(((pyclass *)ptr)->class, ptr, PYSTR(attribref->id->value), values);
    }
    else if (type(targets) == expression_list_t ||
             type(targets) == list_expr_t || type(targets) == parenth_form_t) {
        expression_list *expressions = (expression_list *)targets;
        list *ptr1, *ptr2;
        if (type(values) == pylist_t || type(values) == pytuple_t || type(values) == pyset_t) {
            for (ptr1 = expressions->expr_head,
                 ptr2 = ((pylist *)values)->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next)
                store(env, ptr1->content, ptr2->content);
        }
        else if (type(values) == pyrange_t) {
            pyint *index = int_to_pyint(0);
            pyrange *range = (pyrange *)values;
            for (ptr1 = expressions->expr_head; ptr1; ptr1 = ptr1->next) {
                store(env, ptr1->content, __getitem__(values, index));
                pyint__inc__(index);
            }
        }
    }
}

environment *environment_copy(environment *env) {
    environment *retptr = environment_init(0);
    retptr->val_dict = list_cpy(env->val_dict);
    retptr->outer = env->outer;
    return retptr;
}

void val_dict_entry_del(void *vptr) {
    val_dict_entry *entry = (val_dict_entry *)vptr;
    free(entry->id);
    del(entry->value);
    free(entry);
}

void environment_del(void *vptr) {
    environment *env = (environment *)vptr;
    del(env->val_dict);
    if (env->ret)
        del(env->ret);
    if (env->yield)
        del(env->yield);
    free(env);
}

void del(void *vptr) {
    if (type(vptr) == identifier_t)
        identifier_del(vptr);
    else if (type(vptr) == int_expr_t)
        int_expr_del(vptr);
    else if (type(vptr) == float_expr_t)
        float_expr_del(vptr);
    else if (type(vptr) == imag_expr_t)
        imag_expr_del(vptr);
    else if (type(vptr) == str_expr_t)
        str_expr_del(vptr);
    else if (type(vptr) == parenth_form_t)
        parenth_form_del(vptr);
    else if (type(vptr) == generator_t)
        generator_del(vptr);
    else if (type(vptr) == list_expr_t)
        list_expr_del(vptr);
    else if (type(vptr) == set_expr_t)
        set_expr_del(vptr);
    else if (type(vptr) == dict_expr_t)
        dict_expr_del(vptr);
    else if (type(vptr) == list_comprehension_t)
        list_comprehension_del(vptr);
    else if (type(vptr) == yield_atom_t)
        yield_atom_del(vptr);
    else if (type(vptr) == attributeref_t)
        attributeref_del(vptr);
    else if (type(vptr) == slice_expr_t)
        slice_expr_del(vptr);
    else if (type(vptr) == slicing_t)
        slicing_del(vptr);
    else if (type(vptr) == subsc_expr_t)
        subsc_expr_del(vptr);
    else if (type(vptr) == subscription_t)
        subscription_del(vptr);
    else if (type(vptr) == call_t)
        call_del(vptr);
    else if (type(vptr) == power_t)
        power_del(vptr);
    else if (type(vptr) == u_expr_t)
        u_expr_del(vptr);
    else if (type(vptr) == b_expr_t)
        b_expr_del(vptr);
    else if (type(vptr) == not_test_t)
        not_test_del(vptr);
    else if (type(vptr) == comparison_t)
        comparison_del(vptr);
    else if (type(vptr) == conditional_expression_t)
        conditional_expression_del(vptr);
    else if (type(vptr) == lambda_expr_t)
        lambda_expr_del(vptr);
    else if (type(vptr) == expression_list_t)
        expression_list_del(vptr);

    else if (type(vptr) == expression_stmt_t)
        expression_stmt_del(vptr);
    else if (type(vptr) == assignment_stmt_t)
        assignment_stmt_del(vptr);
    else if (type(vptr) == return_stmt_t)
        return_stmt_del(vptr);
    else if (type(vptr) == yield_stmt_t)
        yield_stmt_del(vptr);
    else if (type(vptr) == break_stmt_t)
        break_stmt_del(vptr);
    else if (type(vptr) == continue_stmt_t)
        continue_stmt_del(vptr);
    else if (type(vptr) == pass_stmt_t)
        pass_stmt_del(vptr);
    else if (type(vptr) == import_stmt_t)
        import_stmt_del(vptr);
    else if (type(vptr) == del_stmt_t)
        del_stmt_del(vptr);
    else if (type(vptr) == stmt_list_t)
        stmt_list_del(vptr);
    else if (type(vptr) == if_stmt_t)
        if_stmt_del(vptr);
    else if (type(vptr) == while_stmt_t)
        while_stmt_del(vptr);
    else if(type(vptr) == for_stmt_t)
        for_stmt_del(vptr);
    else if (type(vptr) == funcdef_t)
        funcdef_del(vptr);
    else if (type(vptr) == classdef_t)
        classdef_del(vptr);
    else if (type(vptr) == suite_t)
        suite_del(vptr);

    else if (ref(vptr) == 0) {
        if (type(vptr) == pyint_t)
            pyint__del__(vptr);
        else if (type(vptr) == pybool_t)
            pybool_del(vptr);
        else if (type(vptr) == pystr_t)
            pystr__del__(vptr);
        else if (type(vptr) == pylist_t)
            pylist_del(vptr);
        else if (type(vptr) == pytuple_t)
            pytuple_del(vptr);
        else if (type(vptr) == pyfunction_t)
            pyfunction_del(vptr);
        else if (type(vptr) == pyclass_t)
            pyclass_del(vptr);

        else if (type(vptr) == list_t)
            list_del(vptr);
        else if (type(vptr) == environment_t)
            environment_del(vptr);
        else if (type(vptr) == val_dict_entry_t)
            val_dict_entry_del(vptr);
    }
    else
        ref_dec(vptr);
}

void store_id(environment *env, char *id, void *value) {
    identifier *_id = IDENTIFIER(id);
    store(env, _id, value);
    del(_id);
}