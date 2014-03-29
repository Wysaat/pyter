#include "../list.h"

int main() {
    list *a = list_node();
    a->content = "hello";
    a->next = list_node();
    a->next->content = "bye";
    list_del(a);
    puts("done");
    return 0;
}