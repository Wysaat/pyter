#include <stdio.h>
#include "../pytype/pystr.h"
#include "../pytype/pyint.h"

int main()
{
	pystr *a = pystr__init__();
	pystr *b = pystr__init__();
	b->value = "hello, bitch";
	a->value = "where";

	pyint *n = pyint__init__();
	n->value = integer__init__("38");
	pystr__puts__(a);
	pystr *c = pystr__mul__(b, n);
	pystr__puts__(c);
	pystr__puts__(pystr__add__(a, b));
	return 0;
}