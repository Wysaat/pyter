#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "list.h"
#include "evaluate.h"

typedef struct identifier identifier;
typedef struct environment environment;

/* CAUTION val_dict could be an empty list */
struct environment {
	int type;
    list *val_dict;
    environment *outer;
    void *ret;
    void *yield;
    int _break;
    int _continue;
};

typedef struct val_dict_entry {
	int type;
    char *id;
    void *value;
} val_dict_entry;

environment *environment_init(environment *outer);
val_dict_entry *val_dict_entry_init(char *id, void *value);
void store(environment *env, void *targets, void *values);
environment *environment_copy(environment *env);
void val_dict_entry_del(void *vptr);
void environment_del(void *vptr);

void del(void *vptr);

void store_id(environment *env, char *id, void *value); // short-hand function for storing identifiers

#endif /* ENVIRONMENT_H */