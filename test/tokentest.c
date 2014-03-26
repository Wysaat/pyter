#include "../scan/token.h"
#include "../scan/buffer.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *a = "374324";
	printf("is_spctk(a): %d\n", is_spctk(a));
	printf("is_strprfx(a): %d\n", is_strprfx(a));
	printf("is_cmpop(a): %d\n", is_cmpop(a));
	printf("is_str(a): %d\n", is_str(a));
	printf("is_int(a): %d\n", is_int(a));
	return 0;
}