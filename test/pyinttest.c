#include <stdio.h>
#include "../pytype/pyint.h"

int main()
{
	pyint *a = pyint__init__();
	a->value = integer__init__("123");
	pyint *zero = pyint__init__();
	zero->value = INTEGER_NODE();
	printf("%d\n", is_true(pyint__gt__(zero, zero)));
	while (is_true(pyint__gt__(a, zero))) {
		pyint__print__(a);
		printf(" ");
		pyint__print__(zero);
		printf(" %d\n", is_true(pyint__gt__(a, zero)));
		pyint__dec__(a);
	}
	pyint__del__(zero);
	pyint__del__(a);
	return 0;
}