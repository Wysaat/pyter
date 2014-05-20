/*
 * CAUTION: list_append_list, list_add SHOULD BE USED CAREFULLY.
 *          Read the code before using the functions.
 */

#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "pytype/methods.h"
#include "pytype/pybool.h"
#include "types.h"

list *list_node() {
    list *retptr = (list *)malloc(sizeof(list));
    retptr->type = list_t;
    retptr->prev = 0;
    retptr->next = 0;
    retptr->content = 0;
    return retptr;
}

list *list_cpy(list *head) {
    list *retptr = list_node();
    if (list_is_empty(head))
        return retptr;
    list *curptr = retptr;
    list *ptr = head;
    while (ptr) {
        curptr->content = ptr->content;
        ptr = ptr->next;
        curptr->next = list_node();
        curptr->next->prev = curptr;
        curptr = curptr->next;
    }
    curptr->prev->next = 0;
    free(curptr);
    return retptr;
}


/* NOT deleting the content */
void list_del(list *head) {
    list *ptr = head, *tmp;
    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
}

list *list_add(list *list1, list *list2) {
    if (list_is_empty(list1))
        return list_cpy(list2);
    if (list_is_empty(list2))
        return list_cpy(list1);
    list *retptr = list_cpy(list1);
    list *to_add = list_cpy(list2);
    list *ptr = retptr;
    while (ptr->next) ptr = ptr->next;
    ptr->next = to_add;
    to_add->prev = ptr;
    return retptr;
}

int list_is_empty(list *head) {
    if (head->prev == 0 && head->next == 0 && head->content == 0)
        return 1;
    return 0;
}

/* list2 is copied and the original list2 is freed */
void list_append_list(list *list1, list *list2) {
    if (!list_is_empty(list2)) {
        if (list_is_empty(list1)) {
            list1->content = list2->content;
            if (list2->next) {
                list1->next = list_cpy(list2->next);
                list1->next->prev = list1;
            }
        }
        else {
            list *ptr = list1;
            while (ptr->next)
                ptr = ptr->next;
            list *to_append = list_cpy(list2);
            ptr->next = to_append;
            to_append->prev = ptr;
        }
    }

    list_del(list2);
}

void list_append_content(list *head, void *content) {
    if (list_is_empty(head))
        head->content = content;
    else {
        list *ptr = head;
        while (ptr->next) ptr = ptr->next;
        ptr->next = list_node();
        ptr->next->content = content;
        ptr->next->prev = ptr;
    }
}

/*
 * default sorting algorithm: quicksort
 * if comp(a, b) <= 0, a in front of b
 * in place sort
 */
void list_sort0(list *head, int comp(), int size) {
    if (list_is_empty(head))
        return;
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
    if (list_is_empty(head))
        return;
    list *ptr = head;
    int size = 0;
    while (ptr) {
        ptr = ptr->next;
        size++;
    }
    list_sort0(head, comp, size);
}

/* the list must contain only pytypes */
int list_find(list *head, void *val) {
    if (list_is_empty(head))
        return -1;
    list *ptr;
    int retval = 0;
    for (ptr = head; ptr; ptr = ptr->next) {
        if (is_true(__eq__(ptr->content, val)))
            return retval;
        retval++;
    }
    return -1;
}

/* the list must contain only pytypes */
int list_eq(list *left, list *right) {
    if (list_is_empty(left) && list_is_empty(right))
        return 1;
    if (list_is_empty(left))
        return 0;
    if (list_is_empty(right))
        return 0;
    list *ptr1 = left, *ptr2 = right;
    while (1) {
        if (!ptr1 || !ptr2) {
            if (ptr1 || ptr2)
                return 0;
            return 1;
        }
        if (!is_true(__eq__(ptr1->content, ptr2->content)))
            return 0;
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
}

void list_replace(list *head, int pos, void *content) {
    int ind;
    list *ptr = head;
    for (ind = 0; ind < pos; ind++)
        ptr = ptr->next;
    free(ptr->content);
    ptr->content = content;
}

int list_len(list *head) {
    if (list_is_empty(head))
        return 0;
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}