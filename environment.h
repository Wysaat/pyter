#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "list.h"
#include "evaluate.h"

typedef struct identifier identifier;
typedef struct environment environment;

/* CAUTION val_dict could be an empty list */
struct environment {
    list *val_dict;
    environment *outer;
    void *ret;
    void *yield;
    int _break;
    int _continue;
};

typedef struct val_dict_entry {
    char *id;
    void *value;
} val_dict_entry;

environment *environment_init(environment *outer);
val_dict_entry *val_dict_entry_init(char *id, void *value);
void store(environment *env, void *targets, void *values);
environment *environment_copy(environment *env);

#endif /* ENVIRONMENT_H */