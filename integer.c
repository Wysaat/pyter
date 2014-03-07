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
    retptr->lower = 0;
    return retptr;
}

integer *integer__init__(mem_block *head) {
    integer *lowest = INTEGER_NODE();
    integer *cur_ptr = lowest;
    int length = mem_size(head);
    char string[INTEGER_SZ+1];
    int copy_sz;
    while (1) {
        if (length < INTEGER_SZ)
            copy_sz = length;
        else
            copy_sz = INTEGER_SZ;
        mem_ncpy_out(string, head, length-copy_sz, copy_sz);
        cur_ptr->value = atoi(string);
        length -= copy_sz;
        if (length == 0)
            return cur_ptr;
        cur_ptr->higher = INTEGER_NODE();
        cur_ptr->higher->lower = cur_ptr;
        cur_ptr = cur_ptr->higher;
    }
}

mem_block *integer__str__(integer *head) {
    mem_block *block = mem_head();
    integer *integer_ptr = head;
    while (integer_ptr) {
        char string[INTEGER_SZ+1];
        char *val = itoa(integer_ptr->value);
        if (integer_ptr == head) {
            strcpy(string, val);
        }
        else {
            int offset = INTEGER_SZ - strlen(val);
            strcpy(string+offset, val);
            string[INTEGER_SZ] = 0;
            while (--offset >= 0)
                string[offset] = '0';
        }
        mem_block *new_block = mem_str(string);
        mem_ncpy(block, new_block, mem_size(block), 0, mem_size(new_block));
        integer_ptr = integer_ptr->lower;
    }
    return block;
}

integer *integer__add__(integer *left, integer *right) {
    integer *lowest = INTEGER_NODE();
    integer *cur_ret = lowest;
    integer *cur_left = left;
    while (cur_left->lower)
        cur_left = cur_left->lower;
    integer *cur_right = right;
    while (cur_right->lower)
        cur_right = cur_right->lower;
    int carry = 0;
    while (1) {
        int den = (int )pow(10, INTEGER_SZ);
        if (cur_left == 0 && cur_right == 0) {
            if (!carry) {
                integer *retptr = cur_ret->lower;
                free(cur_ret);
                return retptr;
            }
            cur_ret->value = carry;
            return cur_ret;
        }
        if (cur_left == 0) {
            cur_ret->value = (cur_right->value + carry) % den;
            carry = (cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_right = cur_right->higher;
        }
        else if (cur_right == 0) {
            cur_ret->value = (cur_left->value + carry) % den;
            carry = (cur_left->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
        }
        else {
            cur_ret->value = (cur_left->value + cur_right->value + carry) % den;
            carry = (cur_left->value + cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
            cur_right = cur_right->higher;
        }
    }
}