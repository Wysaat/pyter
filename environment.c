#include "environment.h"
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "pytype/pylist.h"

environment *environment_init() {
    environment *retptr = (environment *)malloc(sizeof(environment));
    retptr->val_dict = list_node();
    return retptr;
}

val_dict_entry *val_dict_entry_init(char *id, void *value) {
    val_dict_entry *retptr = (val_dict_entry *)malloc(sizeof(val_dict_entry));
    retptr->id = id;
    retptr->value = value;
    return retptr;
}


/* problem is here */
void store(environment *env, void *targets, void *values) {
    if (type(targets) == identifier_t) {
        identifier *id = (identifier *)targets;
        if (list_is_empty(env->val_dict)) {
            list_append_content(env->val_dict, val_dict_entry_init(id->value, values));
            return;
        }
        list *ptr;
        for (ptr = env->val_dict; ptr; ptr = ptr->next) {
            val_dict_entry *entry = (val_dict_entry *)ptr->content;
            if (!strcmp(entry->id, id->value)) {
                /* ...
                Put code to delete old entry->value here.
                ... */
                entry->value = values;
            }
        }
        list_append_content(env->val_dict, val_dict_entry_init(id->value, values));
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
