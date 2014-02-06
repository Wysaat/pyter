#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEMSIZE 1024
#define STRINGSIZE 1024

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

typedef struct {
    char identifier[ITEMSIZE];
} identifier;

identifier *IDENTIFIER(char *item) {
    identifier *retval = (identifier *)malloc(sizeof(identifier));
    strcpy(retval->identifier, item);
    return retval;
}

struct item {
    char content[ITEMSIZE];
    struct item *next;
};

struct {
    char string[STRINGSIZE];
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

void read_numeric_literal(char *item) {
    int i = 0, dot_count = 0;
    while ((global.string[global.index] >= '0' && global.string[global.index] <= '9') ||
                (global.string[global.index] == '.' && !dot_count)) {
        if (global.string[global.index] == '.')
            dot_count++;
        item[i++] = global.string[global.index++];
    }
    item[i] = 0;
    add_item(item);
    return;
}

void read_string_literal(char *item) {
    int i = 1;
    char op = global.string[global.index++];
    item[0] = op;
    while (global.string[global.index] != op)
        item[i++] = global.string[global.index++];
    item[i++] = global.string[global.index++];
    item[i] = 0;
    add_item(item);
    return;
}

void raw_read(char *item) {
    int i, j, k = 0;

    if (global.index < strlen(global.string)) {
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
                read_numeric_literal(item);
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
        if (global.string[global.index] >= '0' && global.string[global.index] <= '9') {
            read_numeric_literal(item);
            return;
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
        for (i = 0; strcmp(stringprefixes[i], " ") != 0; i++) {
            if ((strcmp(stringprefixes[i], item) == 0) && 
                     (global.string[global.index == '"' || global.string[global.index == '\'']])) {
                read_string_literal(item+strlen(item));
                add_item(item);
                return;
            }
            else {
                item[k] = 0;
                add_item(item);
                return;
            }
        }
    }
    else {
        item[0] = 0;
        add_item(item);
        return;
    }
}

int is_digit(char ch) {
    if (ch >= '0' && ch <= '9')
        return 1;
    return 0;
}

int is_alpha(char ch) {
    if ((ch >= 'a' && ch <= 'z') || ch >= 'A' && ch <= 'Z')
        return 1;
    return 0;
}

int is_id(char *item) {
    int i = 0;
    if (!(is_alpha(item[0]) || item[0] == '_'))
        return 0;
    while (item[++i] != 0) {
        if (!(is_alpha(item[i]) || item[i] == '_' || is_digit(item[i])))
            return 0;
    }
    return 1;
}

void *parse_atom() {
    char item[ITEMSIZE];
    raw_read(item);
    if (is_id(item))
        return IDENTIFIER(item);
}

int test1()
{
    interactive_get_line();
    identifier *a = parse_atom();
    printf("identifier: %s\n", a->identifier);

    return 0;
}

int test()
{
    char item[ITEMSIZE];
    do {
        interactive_get_line();
        puts(global.string);
        printf("[");
        while (global.index < strlen(global.string)) {
            raw_read(item);
            printf("'%s', ", item);
        }
        printf("]\n");
    } while (strcmp(global.string, "exit") != 0);

    return 0;
}

int main()
{
    test1();
    return 0;
}