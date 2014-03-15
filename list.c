#include "cpyter.h"

list *list_node() {
    list *retptr = (list *)malloc(sizeof(list));
    retptr->prev = 0;
    retptr->next = 0;
    retptr->content = 0;
    return retptr;
}

int list_is_empty(list *list) {
    if (list->prev == 0 && list->next == 0 && list->content == 0)
        return 1;
    return 0;
}

void list_append_list(list *list1, list *list2) {
    if (list_is_empty(list1))
        memcpy(list1, list2, sizeof(list));
    else {
        list *ptr = list1;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = list2;
        list2->prev = ptr;
    }
}

void list_append_content(list *head, void *content) {
    list *node = list_node();
    node->content = content;
    list_append_list(head, node);
}

list *list_add(list *left, list *right) {
    list *retptr = list_node();
    memcpy(retptr, left, sizeof(list));
    list_append_list(retptr, right);
    return retptr;
}

/*
 * default sorting algorithm: quicksort
 * if comp(a, b) <= 0, a in front of b
 * in place sort
 */
void list_sort0(list *head, int comp(), int size) {
    list *ptr = head->next;
    list *left = head->next, *right;
    list *split = head;
    void *mid_content = head->content;
    int count = 0, lsize = 0, rsize = size - 1;

    while (count++ < size-1) {
        if (comp(ptr->content, mid_content) <= 0) {
            lsize++;
            rsize--;
            split->content = ptr->content;
            list *tmp = ptr;
            while (tmp != split) {
                tmp->content = tmp->prev->content;
                tmp = tmp->prev;
            }
            split = split->next;
        }
        ptr = ptr->next;
    }
    split->content = mid_content;

    if (lsize > 1)
        list_sort0(head, comp, lsize);
    if (rsize > 1)
        list_sort0(split->next, comp, rsize);
}

void list_sort(list *head, int comp()) {
    list *ptr = head;
    int size = 0;
    while (ptr) {
        ptr = ptr->next;
        size++;
    }
    list_sort0(head, comp, size);
}