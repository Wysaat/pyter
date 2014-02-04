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

char stringprefixes[10][2] = { "r", "u", "R", "U", " " };

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

void interactive_get_line() {
    printf(">>> ");
    gets(global.string);
    global.index = 0;
}

void read_numerical_literal(char *item) {
}

void read_string_literal(char *item) {
}

void raw_read(char *item) {
    int i, j, k = 0;

    while (global.string[global.index] == ' ' || global.string[global.index] == '\t') {
        global.index++;
    }
    for (j = 3; j > 0; j--) {
        for (i = 0; strcmp(tokens[i], " ") != 0; i++) {
            if (strncmp(global.string+global.index, tokens[i], j) == 0) {
                strncpy(item, global.string+global.index, j);
                item[j] = 0;
                add_item(item);
                global.index += j;
                return;
            }
        }
    }
    if (global.string[global.index] == '.') {
        if (global.string[global.index+1] >= '0' && global.string[global.index+1] <= '9') {
            read_numerical_literal(item);
            return;
        }
        else {
            strncpy(item, global.string+global.index, 1);
            item[1] = 0;
            add_item(item);
            global.index++;
            return;
        }
    }
    if (global.string[global.index] == '"' || global.string[global.index] == '\'') {
        read_string_literal(item);
        return;
    }
    while ((global.string[global.index] >= 'a' && global.string[global.index] <= 'z') ||
           (global.string[global.index] >= 'A' && global.string[global.index] <= 'Z') ||
           (global.string[global.index] == '_')) {
        item[k++] = global.string[global.index++];
    }
    if (!global.string[global.index]) {
        add_item(item);
        return;
    }
    for (i = 0; strcmp(stringprefixes[i], " ") != 0; i++) {
        if (strcmp(stringprefixes[i], item) == 0) {
            read_string_literal(item+strlen(item));
            return;
        }
        else {
            add_item(item);
            return;
        }
    }
}

int main()
{
    char item[1024];
    bzero(item, 1024);
    do {
        interactive_get_line();
        puts(global.string);
        while (global.index < strlen(global.string)) {
            raw_read(item);
            printf("item: %s\n", item);
            bzero(item, 1024);
        }
        puts("global.items:");
        show_items();
    } while (strcmp(global.string, "exit") != 0);

    return 0;
}