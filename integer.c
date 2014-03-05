#include "cpyter.h"

char *itoa(int value) {
    int n = 0;
    int num = value;
    while (num > 0) {
        num = num / 10;
        n++;
    }
    // char retptr[n+1];
    char *retptr = (char *)malloc(n+1);
    retptr[n] = 0;
    int i;
    for (i = 1; i <= n; i++) {
        retptr[n-i] = value%10 + '0';
        value = value / 10;
    }
    return retptr;
}

integer *INTEGER_NODE() {
    integer *retptr = (integer *)malloc(sizeof(integer));
    retptr->value = 0;
    retptr->higher = 0;
    return retptr;
}

integer *integer__init__(mem_block *head) {
    integer *retptr = INTEGER_NODE();
    integer *cur_ptr = retptr;
    int length = mem_size(head);
    char string[INTEGER_SZ];
    while (1) {
        if (length < INTEGER_SZ)
            copy_sz = length;
        else
            copy_sz = INTEGER_SZ;
        mem_ncpy_out(string, head, length-1-copy_sz, copy_sz);
        cur_ptr->value = atoi(string);
        length -= copy_sz;
        if (length <= 0) {
            return retptr;
        }
        cur_ptr->higher = INTEGER_NODE();
        cur_ptr = cur_ptr->higher;
    }
}

mem_block *integer__str__(integer *head) {
    mem_block *block = mem_head();
    integer *integer_ptr = head;
    while (1) {
        char *tmp = itoa(integer_ptr->value);
        char *string;
        if (strlen(tmp) < INTEGER_SZ) {
            int delta = INTEGER_SZ - tmp;
            string = (char *)malloc(INTEGER_SZ+1);
            int i;
            for (i = 0; i < delta; i++) {
                string[i] = '0';
            }
            strcpy(string+i, tmp);
        }
        else
            string = tmp;
        new_block = mem_str(string);
        mem_ncpy(block, new_block, dest_off, 0, mem_size(new_block));
    }
}

integer *integer__init__str(char *string) {
    integer *retptr = INTEGER_NODE();
    int length = strlen(string);
    int i;
    do {
    }
}

integer *integer__add__(integer *left, integer *right) {
    integer *retptr = INTEGER_NODE();
    integer *cur_ret = retptr;
    integer *cur_left = right;
    integer *cur_right = left;
    int carry = 0;
    while (1) {
        int den = (int )pow(10, INTEGER_SZ);
        if (cur_left == 0 && cur_right == 0) {
            cur_ret->value = carry;
            return retptr;
        }
        if (cur_left == 0) {
            cur_ret->value = (cur_right->value + carry) % den;
            carry = (cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret = cur_ret->higher;
            cur_right = cur_right->higher;
        }
        else if (cur_right == 0) {
            cur_ret->value = (cur_left->value + carry) % den;
            carry = (cur_left->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
        }
        else {
            cur_ret->value = (cur_left->value + cur_right->value + carry) % den;
            carry = (cur_left->value + cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
            cur_right = cur_right->higher;
        }
    }
}