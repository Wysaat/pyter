#ifndef LIST_H
#define LIST_H

/*
 * CAUTION: list_append_list, list_add SHOULD BE USED CAREFULLY.
 */

typedef struct list list;
struct list {
    void *content;
    list *prev;
    list *next;
};

list *list_node();
list *list_cpy(list *);
void list_del(list *);
list *list_add(list *, list *);
int list_is_empty(list *);
void list_append_content(list *, void *);
void list_append_list(list *, list *);
void list_sort(list *, int (*func)());
void list_sort0(list *, int (*func)(), int size);

int list_find(list *, void *);
int list_eq(list *, list *);
void list_replace(list *, int pos, void *);

#endif /* LIST_H */