#include "cpyter.h"

int integer_comp(void *left, void *right) {
	if (integer__lt__((integer *)left, (integer *)right))
		return -1;
	if (integer__eq__((integer *)left, (integer *)right))
		return 0;
	return 1;
}

int main() {
	list *head = list_node();
	integer *a = integer__init__(mem_str("12"));
	integer *b = integer__init__(mem_str("87"));
	integer *c = integer__init__(mem_str("32"));
	integer *d = integer__init__(mem_str("10"));

	list_append_content(head, a);
	list_append_content(head, b);
	list_append_content(head, c);
	list_append_content(head, d);

	int val = integer_comp(head->content, head->next->content);
	printf("val is %d\n", val);

	list_sort(head, integer_comp);
	printf("here\n");
	return 0;
}