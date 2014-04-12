#include "environment.h"
#include "evaluate.h"
#include <string.h>
#include <stdlib.h>

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
void store(environment *env, identifier *id, void *value) {
    if (list_is_empty(env->val_dict)) {
        list_append_content(env->val_dict, val_dict_entry_init(id->value, value));
        return;
    }
    list *ptr;
    for (ptr = env->val_dict; ptr; ptr = ptr->next) {
        // while (1) ;
        val_dict_entry *entry = (val_dict_entry *)ptr->content;
        if (!strcmp(entry->id, id->value)) {
            /* ...
            Put code to delete old entry->value here.
            ... */
            entry->value = value;
        }
    }
    list_append_content(env->val_dict, val_dict_entry_init(id->value, value));
}