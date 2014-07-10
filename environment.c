#include <string.h>
#include <stdlib.h>
#include "environment.h"
#include "types.h"
#include "list.h"
#include "struct_info.h"
#include "evaluate.h"
#include "execute.h"
#include "pytype/methods.h"
#include "pytype/pyint.h"
#include "pytype/pybool.h"
#include "pytype/pyfloat.h"
#include "pytype/pycomplex.h"
#include "pytype/pystr.h"
#include "pytype/pylist.h"
#include "pytype/pytuple.h"
#include "pytype/pyset.h"
#include "pytype/pydict.h"
#include "pytype/pyfunction.h"
#include "pytype/pyclass.h"
#include "pytype/pymodule.h"
#include "pytype/py__builtins__.h"
#include "pytype/others.h"

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
            if (id->value)
                list_append_content(env->val_dict, val_dict_entry_init(strdup(id->value), values));
            else
                list_append_content(env->val_dict, val_dict_entry_init(0, values));
            return;
        }
        list *ptr;
        if (id->value) {
            for (ptr = env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (entry->id && !strcmp(entry->id, id->value)) {
                    entry->value = values;
                    return;
                }
            }
        }
        else { /* nameless identifer, for comprehensions */
            for (ptr = env->val_dict; ptr; ptr = ptr->next) {
                val_dict_entry *entry = (val_dict_entry *)ptr->content;
                if (!entry->id) {
                    entry->value = values;
                    return;
                }
            }
        }
        list_append_content(env->val_dict, val_dict_entry_init(strdup(id->value), values));
    }
    else if (type(targets) == subscription_t) {
        subscription *subsc = (subscription *)targets;
        void *left = evaluate(subsc->primary, env);
        void *right = evaluate(subsc->subsc->value, env);
        __setitem__(left, right, values);
    }
    else if (type(targets) == slicing_t) {
        slicing *slic = (slicing *)targets;
        void *left = evaluate(slic->primary, env);
        void *right = evaluate(slic->slice, env);
        __setitem__(left, right, values);
    }
    else if (type(targets) == attributeref_t) {
        attributeref *attribref = (attributeref *)targets;
        void *ptr = evaluate(attribref->primary, env);
        if (type(ptr) == instance_t)
            __setattr__(((instance *)ptr)->class, ptr, attribref->id->value, values);
        else if (type(ptr) == pyclass_t)
            __setattr__(((pyclass *)ptr)->class, ptr, attribref->id->value, values);
    }
    else if (type(targets) == expression_list_t ||
             type(targets) == list_expr_t || type(targets) == parenth_form_t) {
        expression_list *expressions = (expression_list *)targets;
        list *ptr1, *ptr2;
        if (type(values) == pylist_t || type(values) == pytuple_t || type(values) == pyset_t) {
            for (ptr1 = expressions->expr_head,
                 ptr2 = ((pylist *)values)->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                store(env, ptr1->content, ptr2->content);
            }
        }
        else if (type(values) == pydict_t) {
            for (ptr1 = expressions->expr_head,
                 ptr2 = ((pydict *)values)->keys; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next) {
                store(env, ptr1->content, ptr2->content);
            }
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

void store_id(environment *env, char *id, void *value) {
    identifier *_id = IDENTIFIER(id);
    store(env, _id, value);
}

void *env_find(environment *env, char *name) {
    list *ptr;
    for (ptr = env->val_dict; ptr; ptr = ptr->next) {
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (entry->id && !strcmp(entry->id, name))
            return entry->value;
    }
}

void del(void *vptr) {
    if (!vptr)
        return;
    if (type(vptr) == identifier_t)
        identifier_del(vptr);
    else if (type(vptr) == int_expr_t)
        int_expr_del(vptr);
    else if (type(vptr) == bool_expr_t)
        bool_expr_del(vptr);
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
}