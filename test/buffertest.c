#include <stdio.h>
#include <stdlib.h>
#include "../scan/buffer.h"

int main()
{
	buffer *a = buff_init();
	int i; for (i = 0; i < 100; i++) buff_add(a, 'G');

	char *b = buff_puts(a);
	puts(b);
	printf("buff_len(a) is %d\n", buff_len(a));
	buff_del(a);
	printf("buff_len(a) is %d\n", buff_len(a));

	return 0;
}