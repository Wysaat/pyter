#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpyter.h"

void mem_print(mem_block *block) {
    if (block->mem == 0) {
        puts("NONE");
        return;
    }
    mem_block *ptr;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        printf(ptr->mem);
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
}

void mem_ncpy_out(char *dest, mem_block *block, int offset, int size) {
    mem_block *ptr;
    int count = 0;
    int offset_count = 0, to_read = 0;
    for (ptr = block, ptr != 0; ptr = ptr->next) {
        int sz = strlen(ptr->mem);
        offset_count += sz;
        if (offset_count >= offset) {
            to_read = offset_count - offset;
            if (to_read > size) {
                strncpy(dest, sz-to_read, size);
                return;
            }
            size -= to_read;
            strncpy(dest, sz-to_read, to_read);
            dest += to_read;
            count += to_read;
            ptr = ptr->next;
            break;
        }
    }
    for (; ptr != 0; ptr = ptr->next) {
        int sz = strlen(ptr->mem);
    }

    for (ptr = block; ptr != 0; ptr = ptr->next) {
        int sz = strlen(ptr->mem);
        if (offset_count + sz < offset) {
            offset_count += sz;
            continue;
        }
            to_read = offset_count + sz - offset;
            offset_count = offset;
            if (to_read == 0)
                continue;
        if (size-count < sz) {
            strncpy(dest, ptr->mem, size-count);
            return;
        }
        strncpy(dest, ptr->mem, sz);
        dest += sz;
    }
}