#include "environment.h"
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "pytype/pylist.h"
#include "pytype/methods.h"
#include "pytype/pyclass.h"

environment *environment_init(environment *outer) {
    environment *retptr = (environment *)malloc(sizeof(environment));
    memset(retptr, 0, sizeof(*retptr));
    retptr->val_dict = list_node();
    retptr->outer = outer;
    return retptr;
}

val_dict_entry *val_dict_entry_init(char *id, void *value) {
    val_dict_entry *retptr = (val_dict_entry *)malloc(sizeof(val_dict_entry));
    retptr->id = id;
    retptr->value = value;
    return retptr;
}

void store(environment *env, void *targets, void *values) {
    if (type(targets) == identifier_t) {
        identifier *id = (identifier *)targets;
        if (list_is_empty(env->val_dict)) {
            list_append_content(env->val_dict, val_dict_entry_init(id->value, values));
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
        list_append_content(env->val_dict, val_dict_entry_init(id->value, values));
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
        for (ptr1 = expressions->expr_head,
             ptr2 = ((pylist *)values)->values; ptr1; ptr1 = ptr1->next, ptr2 = ptr2->next)
            store(env, ptr1->content, ptr2->content);
    }
}

environment *environment_copy(environment *env) {
    environment *retptr = environment_init(0);
    retptr->val_dict = list_cpy(env->val_dict);
    retptr->outer = env->outer;
    return retptr;
}
