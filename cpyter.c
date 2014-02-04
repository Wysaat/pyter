#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* "<>" is deprecated in Python 3 */
char tokens[100][4] = { "+", "-", "*", "**", "/", "//", "%",
                        "<<", ">>", "&", "|", "^", "~",
                        "<", ">", "<=", ">=", "==", "!=", "<>",
                        "(", ")", "[", "]", "{", "}", "@",
                        ",", ":", "`", "=", ";",
                        "+=", "-=", "*=", "/=", "//=", "%=",
                        "&=", "|=", "^=", ">>=", "<<=", "**=",

                        "!", "$", "?", "#", "\\",

                        "+=", "-=", "*=", "/=", "//=", "%=", "**=",
                        ">>=", "<<=", "&=", "^=", "|=", " " };

struct item {
    char content[1024];
    struct item *next;
};

struct {
    char string[1024];
    int index;
    int line_number;
    struct item items_head;
} global = {
    .string = "",
    .index = 0,
    .line_number = 0,
    .items_head = { " ", 0 },
};

void add_item(char *word) {
    struct item *ptr = &global.items_head;
    struct item *new_item = (struct item *)malloc(sizeof(struct item));
    strcpy(new_item->content, word);
    new_item->next = 0;
    while (ptr->next != 0)
        ptr = ptr->next;
    ptr->next = new_item;
}

void show_items() {
    struct item *ptr = &global.items_head;
    while (ptr->next != 0) {
        puts(ptr->next->content);
        ptr = ptr->next;
    }
}

void *interactive_get_line() {
    printf(">>> ");
    gets(global.string);
}

void raw_read() {
    int i;
    char *string = global.string;
    int index = global.index;

    while (string[index] == ' ' || string[index] == '\t') {
        index++;
    }
    char item[1024];
    strncpy(item, string + index, 3);
    item[3] = 0;
    for (i = 0; strcmp(tokens[i], " ") != 0; i++) {
        if (strcmp(item, tokens[i]) == 0) {
            add_item(item);
            index += 3;
            // global.index = index;
            return;
        }
    }
}

int main()
{
    do {
        interactive_get_line();
        puts(global.string);
        raw_read();
        show_items();
    } while (strcmp(global.string, "exit") != 0);

    return 0;
}