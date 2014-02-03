#include <stdio.h>
#include <string.h>

struct {
    char string[1024];
    int index;
    int line_number;
} global = {
    .string = "hello, world",
    .index = 0,
    .line_number = 0,
};

int line_number;

// "<>" is deprecated in Python 3
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

char *interactive_get_line() {
    char string[1024];
    char *bitch = "hell";
    printf(">>> ");
    gets(string);
    return string;
}

void raw_read(char *string, int index) {
    int i;
    while (string[index] == ' ' || string[index] == '\t') {
        index ++;
    }
    char item[1024];
    strncpy(item, string + index, 3);
    for (i = 0; strcmp(tokens[i], " ") != 0; i++) {
        if (strcmp(item, tokens[i]) == 0)
            return 0;
    }
    puts(item);
    return 0;
}

char *string;

int main()
{
    char *index = "1024";
    puts(index);
    puts(global.string);
    do {
        string = interactive_get_line();
        puts(string);
    } while (strcmp(string, "exit") != 0);

    return 0;
}