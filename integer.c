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
    char *retptr;
    if (num == 0) {
        retptr = (char *)malloc(2);
        retptr[0] = '0';
        retptr[1] = 0;
        return retptr;
    }
    while (num > 0) {
        num = num / 10;
        n++;
    }
    // char retptr[n+1];
    retptr = (char *)malloc(n+1);
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

integer *integer__init__(string *strval) {
    integer *lowest = INTEGER_NODE();
    integer *cur_ptr = lowest;
    int length = string_len(strval);
    char chars[INTEGER_SZ+1];
    int copy_sz, i = 0;
    while (1) {
        copy_sz = (length < INTEGER_SZ) ? length : INTEGER_SZ;
        chars = string_tochs(string_slice(strval, ));
        cur_ptr->value = atoi(chars);
        free(chars);
        cur_ptr->index = i++;
        length -= copy_sz;
        if (length == 0)
            return cur_ptr;
        cur_ptr->higher = INTEGER_NODE();
        cur_ptr->higher->lower = cur_ptr;
        cur_ptr = cur_ptr->higher;
    }
}

void integer__del__(integer *head) {
    integer *ptr = head, *lower;
    while (ptr) {
        lower = ptr->lower;
        free(ptr);
        ptr = lower;
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

integer *integer__abs__(integer *head) {
    integer *retptr = integer__cpy__(head);
    retptr->sign = '+';
    return retptr;
}

integer *integer__invert__(integer *head) {
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *retptr = integer__add__(head, one);
    retptr = integer__neg__(retptr);
    return retptr;
}

string *integer__str__(integer *head) {
    string *strptr = string_init(), *new_strptr, *to_add;
    integer *integer_ptr = head;
    char *val;
    while (integer_ptr) {
        char chars[INTEGER_SZ+1];
        val = itoa(integer_ptr->value);
        if (integer_ptr == head) {
            if (integer_ptr->sign == '-') {
                string[0] = integer_ptr->sign;
                strcpy(chars+1, val);
                free(val);
            }
            else {
                strcpy(chars, val);
                free(val);
            }
        }
        else {
            int offset = INTEGER_SZ - strlen(val);
            strcpy(chars+offset, val);
            free(val);
            chars[INTEGER_SZ] = 0;
            while (--offset >= 0)
                string[offset] = '0';
        }
        to_add = string_frchs(chars);
        new_strptr = string_add(strptr, to_add);
        string_del(to_add);
        string_del(new_strptr);
        strptr = new_strptr;
        integer_ptr = integer_ptr->lower;
    }
    return strptr;
}

integer *integer__inc__(integer *head) {
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *retptr = integer__add__(head, one);
    free(one);
    return retptr;
}

integer *integer__dec__(integer *head) {
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *retptr = integer__sub__(head, one);
    free(one);
    return retptr;
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
    string *strl = integer__str__(left);
    string *strr = integer__str__(right);
    if (string_len(strl) > string_len(strr)) {
        string_del(strl);
        string_del(strr);
        return 1;
    }
    else if (string_len(strl) < string_len(strr)) {
        string_del(strl);
        string_del(strr);
        return 0;
    }
    int i, size = string_len(strl);
    char *l, *r;
    for (i = 0; i < size; i++) {
        l = string_ssci(strl, i);
        r = string_ssci(strr, i);
        if (*l > *r)
            return 1;
        else if (*l < *r)
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

int integer__cmp__(integer *left, integer *right) {
    if (integer__lt__(left, right))
        return -1;
    if (integer__eq__(left, right))
        return 0;
    return 1;
}

integer *integer__add__(integer *left, integer *right) {
    integer *retptr, *left_neg, *right_neg;

    if (left->sign == '+' && right->sign == '-') {
        right_neg = integer__neg__(right);
        retptr = integer__sub__(left, right_neg);
        integer__del__(right_neg);
        return retptr;
    }
    else if (left->sign == '-' && right->sign == '+') {
        left_neg = integer__neg__(left);
        retptr = integer__neg__(integer__sub__(left_neg, right));
        integer__del__(left_neg);
        return retptr;
    }
    else if (left->sign == '-' && right->sign == '-') {
        left_neg = integer__neg__(left);
        right_neg = integer__neg__(right);
        retptr = integer__neg__(integer__add__(left_neg, right_neg));
        integer__del__(left_neg);
        integer__del__(right_neg);
        return retptr;
    }

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
                retptr = cur_ret->lower;
                retptr->higher = 0;
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

/* must copy left */
integer *integer__sub__(integer *left, integer *right) {
    integer *retptr, *left_neg, *right_neg;
    if (left->sign == '+' && right->sign == '-') {
        right_neg = integer__neg__(right);
        retptr = integer__add__(left, right_neg);
        integer__del__(right_neg);
        return retptr;
    }
    else if (left->sign == '-' && right->sign == '+') {
        left_neg = integer__neg__(left);
        retptr = integer__neg__(integer__add__(left_neg, right));
        integer__del__(left_neg);
        return retptr;
    }
    else if (left->sign == '-' && right->sign == '-') {
        left_neg = integer__neg__(left);
        right_neg = integer__neg__(right);
        retptr = integer__neg__(integer__sub__(left_neg, right_neg));
        integer__del__(left_neg);
        integer__del__(right_neg);
        return retptr;
    }
    else if (integer__lt__(left, right)) {
        return integer__neg__(integer__sub__(right, left));
    }

    integer *left_copy = integer__cpy__(left);

    integer *lowest = INTEGER_NODE();
    integer *cur_ret = lowest;
    integer *cur_left = left_copy;
    while (cur_left->lower)
        cur_left = cur_left->lower;
    integer *cur_right = right;
    while (cur_right->lower)
        cur_right = cur_right->lower;
    /* BIG BUG:
     * WRONG: !!!! int carry = (int )pow(10, INTEGER_SZ+1), i = 0;
     */
    int carry = (int )pow(10, INTEGER_SZ), i = 0;
    // printf("cur_left->value: %d\n", cur_left->value);
    // printf("cur_right->value: %d\n", cur_right->value);

    while (1) {
        if (cur_left == 0) {
            integer *tmp = cur_ret->lower;
            tmp->higher = 0;
            free(cur_ret);
            free(left_copy);
            cur_ret = tmp;
            while (tmp->value == 0) {
                if (tmp->lower == 0)
                    return tmp;
                cur_ret = tmp->lower;
                cur_ret->higher = 0;
                free(tmp);
                tmp = cur_ret;
            }
            return cur_ret;
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
    retptr->higher = 0;
    free(node_ptr);
    return retptr;
}

integer *integer__node__mul__(integer *node1, integer *node2) {
    integer *retptr = integer__mkempt__(node1->index+node2->index+2);
    long long value = (long long )node1->value * (long long )node2->value;
    char *val_char = lltoa(value);
    string *val_block = string_frchs(val_char);
    free(val_char);
    integer *valptr = integer__init__(val_block);
    string_del(val_block);
    if (valptr->index == 1) {
        retptr->value = valptr->value;
        retptr->lower->value = valptr->lower->value;
        return retptr;
    }
    else {
        retptr->lower->value = valptr->value;
        retptr->lower->higher = 0;
        integer *ret = retptr->lower;
        free(retptr);
        return ret;
    }
}

integer *integer__mul__(integer *left, integer *right) {
    integer *retptr = INTEGER_NODE(), *leftptr, *rightptr, *new_retptr;
    for (leftptr = left; leftptr; leftptr = leftptr->lower) {
        for (rightptr = right; rightptr; rightptr = rightptr->lower) {
            integer *val = integer__node__mul__(leftptr, rightptr);
            val->higher = 0;
            new_retptr = integer__add__(retptr, val);
            integer__del__(val);
            integer__del__(retptr);
            retptr = new_retptr;
        }
    }
    if (left->sign == right->sign)
        retptr->sign = '+';
    else
        retptr->sign = '-';
    return retptr;
}

/* too slow */
integer *integer__div__(integer *left, integer *right) {
    integer *retptr = INTEGER_NODE(), *tmp;
    integer *left_abs = integer__abs__(left), *right_abs = integer__abs__(right), *new_left_abs;
    // int counter = 1;
    while (integer__ge__(left_abs, right_abs)) {
        new_left_abs = integer__sub__(left_abs, right_abs);
        integer__del__(left_abs);
        left_abs = new_left_abs;
        tmp = integer__inc__(retptr);
        integer__del__(retptr);
        retptr = tmp;
    }
    integer__del__(right_abs);
    if (left->sign != right->sign) {
        integer *zero = INTEGER_NODE();
        if (!integer__eq__(left_abs, zero)) {
            integer__del__(zero);
            integer *ret = integer__inc__(retptr);
            integer *ret_neg = integer__neg__(ret);
            integer__del__(left_abs);
            integer__del__(retptr);
            integer__del__(ret);
            return ret_neg;
        }
        else {
            integer__del__(zero);
            integer *ret = integer__neg__(retptr);
            integer__del__(retptr);
            return ret;
        }
    }
    integer__del__(left_abs);
    return retptr;
}

integer *integer__mod__(integer *left, integer *right) {
    integer *lcopy = integer__cpy__(left), *tmp;
    while (integer__ge__(lcopy, right)) {
        tmp = integer__sub__(lcopy, right);
        integer__del__(lcopy);
        lcopy = tmp;
    }
    return lcopy;
}

/* buggy: (a) cannot handle big numbers, (b) get wrong with some big numbers */
integer *integer__pow__(integer *left, integer *right) {
    integer *retptr = INTEGER_NODE();
    retptr->value = 1;
    integer *i, *new_retptr, *new_i;
    i = INTEGER_NODE();
    while (integer__lt__(i, right)) {
        new_retptr = integer__mul__(retptr, left);
        retptr = new_retptr;
        new_i = integer__inc__(i);
        i = new_i;
    }
    return retptr;
}

integer *integer__lshift__(integer *left, integer *right) {
    integer *two = INTEGER_NODE();
    two->value = 2;
    integer *power = integer__pow__(two, right);
    free(two);
    integer *retptr = integer__mul__(left, power);
    integer__del__(power);
    return retptr;
}

integer *integer__rshift__(integer *left, integer *right) {
    integer *two = INTEGER_NODE();
    two->value = 2;
    integer *power = integer__pow__(two, right);
    free(two);
    integer *retptr = integer__div__(left, power);
    integer__del__(power);
    return retptr;
}

integer *integer__and__(integer *left, integer *right) {
    integer *two = INTEGER_NODE();
    two->value = 2;
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *zero = INTEGER_NODE();
    integer *retptr = INTEGER_NODE(), *new_retptr, *power;
    integer *lcopy = integer__cpy__(left), *rcopy = integer__cpy__(right);
    integer *n = INTEGER_NODE(), *lmod, *rmod, *n_new, *new_lcopy, *new_rcopy;
    int lm, rm;
    while (!integer__eq__(lcopy, zero) && !integer__eq__(rcopy, zero)) {
        lmod = integer__mod__(lcopy, two);
        rmod = integer__mod__(rcopy, two);
        if (integer__eq__(lmod, one))
            lm = 1;
        else
            lm = 0;
        if (integer__eq__(rmod, one))
            rm = 1;
        else
            rm = 0;
        if (lm & rm) {
            power = integer__pow__(two, n);
            new_retptr = integer__add__(retptr, power);
            integer__del__(power);
            integer__del__(retptr);
            retptr = new_retptr;
        }
        n_new = integer__inc__(n);
        integer__del__(n);
        n = n_new;
        new_lcopy = integer__div__(lcopy, two);
        new_rcopy = integer__div__(rcopy, two);
        integer__del__(lcopy);
        integer__del__(rcopy);
        lcopy = new_lcopy;
        rcopy = new_rcopy;
    }
    free(one);
    free(two);
    free(zero);
    return retptr;
}

integer *integer__xor__(integer *left, integer *right) {
    integer *two = INTEGER_NODE();
    two->value = 2;
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *zero = INTEGER_NODE();
    integer *retptr = INTEGER_NODE(), *new_retptr, *power;
    integer *lcopy = integer__cpy__(left), *rcopy = integer__cpy__(right);
    integer *n = INTEGER_NODE(), *lmod, *rmod, *n_new, *new_lcopy, *new_rcopy;
    int lm, rm;
    while (!(integer__eq__(lcopy, zero) && integer__eq__(rcopy, zero))) {
        lmod = integer__mod__(lcopy, two);
        rmod = integer__mod__(rcopy, two);
        if (integer__eq__(lmod, one))
            lm = 1;
        else
            lm = 0;
        if (integer__eq__(rmod, one))
            rm = 1;
        else
            rm = 0;
        if (lm ^ rm) {
            power = integer__pow__(two, n);
            new_retptr = integer__add__(retptr, power);
            integer__del__(power);
            integer__del__(retptr);
            retptr = new_retptr;
        }
        n_new = integer__inc__(n);
        integer__del__(n);
        n = n_new;
        new_lcopy = integer__div__(lcopy, two);
        new_rcopy = integer__div__(rcopy, two);
        integer__del__(lcopy);
        integer__del__(rcopy);
        lcopy = new_lcopy;
        rcopy = new_rcopy;
    }
    free(one);
    free(two);
    free(zero);
    return retptr;
}

integer *integer__or__(integer *left, integer *right) {
    integer *two = INTEGER_NODE();
    two->value = 2;
    integer *one = INTEGER_NODE();
    one->value = 1;
    integer *zero = INTEGER_NODE();
    integer *retptr = INTEGER_NODE(), *new_retptr, *power;
    integer *lcopy = integer__cpy__(left), *rcopy = integer__cpy__(right);
    integer *n = INTEGER_NODE(), *lmod, *rmod, *n_new, *new_lcopy, *new_rcopy;
    int lm, rm;
    while (!(integer__eq__(lcopy, zero) && integer__eq__(rcopy, zero))) {
        lmod = integer__mod__(lcopy, two);
        rmod = integer__mod__(rcopy, two);
        if (integer__eq__(lmod, one))
            lm = 1;
        else
            lm = 0;
        if (integer__eq__(rmod, one))
            rm = 1;
        else
            rm = 0;
        if (lm | rm) {
            power = integer__pow__(two, n);
            new_retptr = integer__add__(retptr, power);
            integer__del__(power);
            integer__del__(retptr);
            retptr = new_retptr;
        }
        n_new = integer__inc__(n);
        integer__del__(n);
        n = n_new;
        new_lcopy = integer__div__(lcopy, two);
        new_rcopy = integer__div__(rcopy, two);
        integer__del__(lcopy);
        integer__del__(rcopy);
        lcopy = new_lcopy;
        rcopy = new_rcopy;
    }
    free(one);
    free(two);
    free(zero);
    return retptr;
}