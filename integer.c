#include "cpyter.h"

char *itoa(int value) {
    int n = 0;
    int num = value;
    if (num == 0) {
        return "0";
    }
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

char *lltoa(long long value) {
    int n = 0;
    long long num = value;
    if (num == 0) {
        return "0";
    }
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
    retptr->sign = '+';
    retptr->index = 0;
    return retptr;
}

integer *integer__init__(mem_block *head) {
    integer *lowest = INTEGER_NODE();
    integer *cur_ptr = lowest;
    int length = mem_size(head);
    char string[INTEGER_SZ+1];
    int copy_sz, i = 0;
    while (1) {
        if (length < INTEGER_SZ)
            copy_sz = length;
        else
            copy_sz = INTEGER_SZ;
        mem_ncpy_out(string, head, length-copy_sz, copy_sz);
        cur_ptr->value = atoi(string);
        cur_ptr->index = i++;
        length -= copy_sz;
        if (length == 0)
            return cur_ptr;
        cur_ptr->higher = INTEGER_NODE();
        cur_ptr->higher->lower = cur_ptr;
        cur_ptr = cur_ptr->higher;
    }
}

integer *integer__cpy__(integer *head) {
    integer *ret_head = INTEGER_NODE();
    ret_head->sign = head->sign;
    integer *ptr;
    integer *retptr = ret_head;
    for (ptr = head; ptr != 0; ptr = ptr->lower) {
        retptr->value = ptr->value;
        retptr->index = ptr->index;
        retptr->lower = INTEGER_NODE();
        retptr->lower->higher = retptr;
        retptr = retptr->lower;
    }
    retptr->higher->lower = 0;
    free(retptr);
    return ret_head;
}

integer *integer__neg__(integer *head) {
    integer *retptr = integer__cpy__(head);
    if (head->sign == '+')
        retptr->sign = '-';
    else
        retptr->sign = '+';
    return retptr;
}

mem_block *integer__str__(integer *head) {
    mem_block *block = mem_head();
    integer *integer_ptr = head;
    while (integer_ptr) {
        char string[INTEGER_SZ+1];
        char *val = itoa(integer_ptr->value);
        if (integer_ptr == head) {
            string[0] = integer_ptr->sign;
            strcpy(string+1, val);
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

int integer__eq__(integer *left, integer *right) {
    if (integer__gt__(left, right))
        return 0;
    else if (integer__lt__(left, right))
        return 0;
    return 1;
}

int integer__gt__(integer *left, integer *right) {
    if (left->sign == '+' && right->sign == '-')
        return 1;
    else if (left->sign == '-' && right->sign == '+')
        return 0;
    else if (left->sign == '-' && right->sign == '-')
        return 1 - integer__gt__(integer__neg__(left), integer__neg__(right));
    mem_block *blkl = integer__str__(left);
    mem_block *blkr = integer__str__(right);
    if (mem_size(blkl) > mem_size(blkr))
        return 1;
    else if (mem_size(blkl) < mem_size(blkr))
        return 0;
    int i, size = mem_size(blkl);
    for (i = 0; i < size; i++) {
        if (mem_subscription(blkl, i) > mem_subscription(blkr, i))
            return 1;
        else if (mem_subscription(blkl, i) < mem_subscription(blkr, i))
            return 0;
    }
    return 0;
}

int integer__lt__(integer *left, integer *right) {
    return integer__gt__(right, left);
}

int integer__ge__(integer *left, integer *right) {
    return !integer__lt__(left, right);
}

int integer__le__(integer *left, integer *right) {
    return !integer__gt__(left, right);
}

integer *integer__add__(integer *left, integer *right) {
    if (left->sign == '+' && right->sign == '-')
        return integer__sub__(left, integer__neg__(right));
    else if (left->sign == '-' && right->sign == '+')
        return integer__neg__(integer__sub__(integer__neg__(left), right));
    else if (left->sign == '-' && right->sign == '-')
        return integer__neg__(integer__add__(integer__neg__(left), integer__neg__(right)));
    integer *lowest = INTEGER_NODE();
    integer *cur_ret = lowest;
    integer *cur_left = left;
    while (cur_left->lower)
        cur_left = cur_left->lower;
    integer *cur_right = right;
    while (cur_right->lower)
        cur_right = cur_right->lower;
    int carry = 0, i = 0;
    while (1) {
        int den = (int )pow(10, INTEGER_SZ);
        if (cur_left == 0 && cur_right == 0) {
            if (!carry) {
                integer *retptr = cur_ret->lower;
                free(cur_ret);
                return retptr;
            }
            cur_ret->value = carry;
            cur_ret->index = i++;
            return cur_ret;
        }
        if (cur_left == 0) {
            cur_ret->value = (cur_right->value + carry) % den;
            cur_ret->index = i++;
            carry = (cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_right = cur_right->higher;
        }
        else if (cur_right == 0) {
            cur_ret->value = (cur_left->value + carry) % den;
            cur_ret->index = i++;
            carry = (cur_left->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
        }
        else {
            cur_ret->value = (cur_left->value + cur_right->value + carry) % den;
            cur_ret->index = i++;
            carry = (cur_left->value + cur_right->value + carry) / den;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
            cur_right = cur_right->higher;
        }
    }
}

integer *integer__sub__(integer *left, integer *right) {
    if (left->sign == '+' && right->sign == '-')
        return integer__add__(left, integer__neg__(right));
    else if (left->sign == '-' && right->sign == '+')
        return integer__neg__(integer__add__(integer__neg__(left), right));
    else if (left->sign == '-' && right->sign == '-')
        return integer__neg__(integer__sub__(integer__neg__(left), integer__neg__(right)));
    else if (integer__lt__(left, right))
        return integer__neg__(integer__sub__(right, left));
    integer *lowest = INTEGER_NODE();
    integer *cur_ret = lowest;
    integer *cur_left = left;
    while (cur_left->lower)
        cur_left = cur_left->lower;
    integer *cur_right = right;
    while (cur_right->lower)
        cur_right = cur_right->lower;
    int carry = (int )pow(10, INTEGER_SZ+1), i = 0;
    while (1) {
        if (cur_left == 0) {
            integer *retptr = cur_ret->lower;
            free(cur_ret);
            return retptr;
        }
        else if (cur_right == 0) {
            cur_ret->value = cur_left->value;
            cur_ret->index = i++;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
        }
        else {
            if (cur_left->value < cur_right->value) {
                cur_ret->value = carry + cur_left->value - cur_right->value;
                cur_left->higher->value--;
            }
            else {
                cur_ret->value = cur_left->value - cur_right->value;
            }
            cur_ret->index = i++;
            cur_ret->higher = INTEGER_NODE();
            cur_ret->higher->lower = cur_ret;
            cur_ret = cur_ret->higher;
            cur_left = cur_left->higher;
            cur_right = cur_right->higher;
        }
    }
}

integer *integer__mkempt__(int size) {
    integer *lowest_node = INTEGER_NODE();
    integer *node_ptr = lowest_node;
    int i;
    for (i = 0; i < size; i++) {
        node_ptr->index = i;
        node_ptr->higher = INTEGER_NODE();
        node_ptr->higher->lower = node_ptr;
        node_ptr = node_ptr->higher;
    }
    integer *retptr = node_ptr->lower;
    free(node_ptr);
    return retptr;
}

integer *integer__node__mul__(integer *node1, integer *node2) {
    integer *retptr = integer__mkempt__(node1->index+node2->index+2);
    long long value = (long long )node1->value * (long long)node2->value;
    char *val_char = lltoa(value);
    mem_block *val_block = mem_str(val_char);
    integer *valptr = integer__init__(val_block);
    if (valptr->index == 1) {
        retptr->value = valptr->value;
        retptr->lower->value = valptr->lower->value;
    }
    else {
        retptr->lower->value = valptr->value;
    }
    printf("retptr is: ");
    mem_print(integer__str__(retptr));
    return retptr;
}

integer *integer__mul__(integer *left, integer *right) {
    integer *retptr = INTEGER_NODE(), *leftptr, *rightptr;
    for (leftptr = left; leftptr; leftptr = leftptr->lower) {
        for (rightptr = right; rightptr; rightptr = rightptr->lower) {
            integer *val = integer__node__mul__(leftptr, rightptr);
            val->higher = 0;
            retptr = integer__add__(retptr, val);
        }
    }
    if (left->sign == right->sign)
        retptr->sign = '+';
    else
        retptr->sign = '-';
    return retptr;
}