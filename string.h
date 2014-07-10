#ifndef STRING_H
#define STRING_H

char *stradd(char *left, char *right);
char *stradd2(char *left, char *right);
char *strslice(char *val, int start, int stop, int step);

typedef struct buffer2 buffer2;
struct buffer2 {
    char *content;
    buffer2 *prev;
    buffer2 *next;
};

buffer2 *buff2_init();
void *buff2_add(buffer2 *buff, char *value);
char *buff2_puts(buffer2 *buff);
void buff2_del(buffer2 *buff);
char *buff2_puts2(buffer2 *buff);

#endif /* STRING_H */