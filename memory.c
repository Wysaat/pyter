#include "cpyter.h"

void mem_print_nnl(mem_block *block) {
    if (block->mem == 0) {
        puts("mem_print: NONE");
        return;
    }
    mem_block *ptr;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        printf("%s", ptr->mem);
    }
}

void mem_print(mem_block *block) {
    mem_print_nnl(block);
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
        if ((index-count) < strlen(ptr->mem)) {
            return ptr->mem+index-count;
        }
        count += strlen(ptr->mem);
    }
    return 0;
}

/* python convention, start is included, stop excluded */
mem_block *mem_slice(mem_block *block, int start, int stop) {
    mem_block *retptr = mem_head();
    mem_ncpy(retptr, block, 0, start, stop-start);
    return retptr;
}

void mem_alloc(mem_block *block) {
    mem_block *new_block = (mem_block *)malloc(sizeof(mem_block));
    new_block->prev = block;
    new_block->next = 0;
    block->next = new_block;
    bzero(new_block->mem, MEM_BLOCK_SZ);
}

void mem_set(mem_block *block, int index, char val) {
    if ((index+1) <= mem_size(block)) {
        char *addr = mem_get(block, index);
        strncpy(addr, &val, 1);
    }
    else if (index == mem_size(block)) {
        mem_block *ptr = block;
        while (ptr->next)
            ptr = ptr->next;
        if (strlen(ptr->mem) == MEM_BLOCK_SZ-1) {
            ptr->next = mem_head();
            ptr = ptr->next;
        }
        ptr->mem[strlen(ptr->mem)] = val;
    }
}

void mem_del(mem_block *block, int index) {
    if ((index+1) > mem_size(block))
        return;
    mem_block *ptr;
    int cnt = 0;
    for (ptr = block; ptr != 0; ptr = ptr->next) {
        if ((cnt+strlen(ptr->mem)) >= (index+1)) {
            if (strlen(ptr->mem) == 1) {
                ptr->prev->next = ptr->next;
                if (ptr->next != 0)
                    ptr->next->prev = ptr->prev;
                free(ptr);
                return;
            }
            int i;
            for (i = index-cnt; i < strlen(ptr->mem); i++) {
                ptr->mem[i] = ptr->mem[i+1];
            }
            return;
        }
        cnt += strlen(ptr->mem);
    }
}

// python convention, start is included, stop isn't
void mem_delete(mem_block *block, int start, int stop) {
    int i;
    for (i = stop-start; i > 0; i--) {
        mem_del(block, start);
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
    int d, s, cnt;
    for (d = dest_off, s = src_off, cnt = 0; cnt < size; d++, s++, cnt++) {
        char val = mem_subscription(src, s);
        mem_set(dest, d, val);
    }
}

/* dest_off is the address the first byte of src will be in */
void mem_insert(mem_block *dest, mem_block *src, int dest_off, int src_off, int size) {
    mem_block *new_block = mem_head();
    mem_ncpy(new_block, dest, 0, dest_off, mem_size(dest)-dest_off);
    mem_delete(dest, dest_off, mem_size(dest));
    mem_ncpy(dest, src, dest_off, src_off, size);
    mem_ncpy(dest, new_block, mem_size(dest), 0, mem_size(new_block));
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

void mem_free(mem_block *head) {
    mem_block *ptr = head, *tmp;
    while (ptr) {
        tmp = ptr->next;
        free(ptr);
        ptr = tmp;
    }
}