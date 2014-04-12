#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "list.h"

typedef struct environment {
    list *val_dict;
} environment;

typedef struct val_dict_entry {
    char *id;
    void *value;
} val_dict_entry;

environment *environment_init();
val_dict_entry *val_dict_entry_init(char *id, void *value);

#endif /* ENVIRONMENT_H */