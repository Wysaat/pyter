#define BUFF_SZ 512;

typedef struct buffer buffer;

struct buffer {
    char *value;
    buffer *prev;
    buffer *next;
};

buffer *buff_init();
void buff_del(buffer *);
void buff_add(buffer *, char); /* add a character to the buffer */
int buff_len(buffer *);