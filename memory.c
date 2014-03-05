#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpyter.h"

void mem_print(mem_block *block) {
    if (block->mem == 0) {
        puts("mem_print: NONE");
        return;
    }
    mem_block *ptr;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        printf("%s", ptr->mem);
    }
    printf("\n");
}

mem_block *mem_head() {
    mem_block *retptr = (mem_block *)malloc(sizeof(mem_block));
    bzero(retptr, sizeof(mem_block));
    return retptr;
}

char mem_subscription(mem_block *block, int index) {
    mem_block *ptr;
    int count = 0;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        int sz = index - count;
        if (sz < strlen(ptr->mem)) {
            return ptr->mem[sz];
        }
        count += strlen(ptr->mem);
    }
    return 0;
}

char *mem_get(mem_block *block, int index) {
    mem_block *ptr;
    int count = 0;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        int sz = index - count;
        if (sz < strlen(ptr->mem)) {
            return ptr->mem + sz;
        }
        count += strlen(ptr->mem);
    }
    return 0;
}

void mem_alloc(mem_block *block) {
    mem_block *new_block = (mem_block *)malloc(sizeof(mem_block));
    new_block->prev = block;
    new_block->next = 0;
    block->next = new_block;
    bzero(new_block->mem, MEM_BLOCK_SZ);
}

void mem_set(mem_block *block, int index, char val) {
    mem_block *ptr = block;
    int count = 0, sz, i;
    char space = ' ';
    while (1) {
        sz = index - count;
        if (sz < MEM_BLOCK_SZ-1) {
            if (sz < strlen(ptr->mem)) {
                strncpy(ptr->mem+sz, &val, 1);
                // ptr->mem[sz] = val;
                return;
            }
            for (i = strlen(ptr->mem); i < sz; i++)
                strncpy(ptr->mem+i, &space, 1);
            strncpy(ptr->mem+sz, &val, 1);
            return;
        }
        if (strlen(ptr->mem) < MEM_BLOCK_SZ-1) {
            for (i = strlen(ptr->mem); i < MEM_BLOCK_SZ-1; i++)
                strncpy(ptr->mem+i, &space, 1);
        }
        count += MEM_BLOCK_SZ-1;
        if (ptr->next == 0) {
            mem_alloc(ptr);
        }
        ptr = ptr->next;
    }
}

int mem_size(mem_block *block) {
    mem_block *ptr;
    int retval = 0;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        retval += strlen(ptr->mem);
    }
    return retval;
}

void mem_ncpy_out(char *dest, mem_block *block, int offset, int size) {
    mem_block *ptr;
    int count = 0;
    int offset_count = 0, to_read = 0;

    for (ptr = block; ptr != 0; ptr = ptr->next) {
        int sz = strlen(ptr->mem);
        offset_count += sz;
        if (offset_count >= offset) {
            to_read = offset_count - offset;
            if (to_read > size) {
                strncpy(dest, ptr->mem+sz-to_read, size);
                dest[size] = 0;
                return;
            }
            strncpy(dest, ptr->mem+sz-to_read, to_read);
            dest += to_read;
            count += to_read;
            if (ptr->next == 0) {
                *dest = 0;
                return;
            }
            ptr = ptr->next;
            break;
        }
    }

    for (; ptr != 0; ptr = ptr->next) {
        int sz = strlen(ptr->mem);
        if (size-count < sz) {
            strncpy(dest, ptr->mem, size-count);
            dest[size-count] = 0;
            return;
        }
        strncpy(dest, ptr->mem, sz);
        count += sz;
        dest += sz;
    }
}

void mem_ncpy(mem_block *dest, mem_block *src, int dest_off, int src_off, int size) {
    int d, s ,cnt;
    for (d = dest_off, s = src_off, cnt = 0; cnt < size; d++, s++, cnt++) {
        char val = mem_subscription(src, s);
        mem_set(dest, d, val);
    }
}

void mem_cpy(mem_block *dest, mem_block *src) {
    mem_ncpy(dest, src, 0, 0, mem_size(src));
}

int mem_ncmp(char *dest, mem_block *block, int offset, int size) {
    char *tmp = (char *)malloc(size);
    mem_ncpy_out(tmp, block, offset, size);
    int retval = strncmp(dest, tmp, size);
    free(tmp);
    return retval;
}

int mem_match_str(mem_block *block, char *dest) {
    return !mem_ncmp(dest, block, 0, strlen(dest));
}

// construct mem from char *
mem_block *mem_str(char *content) {
    mem_block *head = mem_head();
    int size = strlen(content), i;
    for (i = 0; i < size; i++) {
        /* mem_set NOT memset */
        mem_set(head, i, content[i]);
    }
    return head;
}
