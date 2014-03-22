#include "cpyter.h"

pyint *pyint__init__() {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    return retptr;
}

pyint *pyint__add__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__add__(left->value, right->value);
    return retptr;
}

pyint *pyint__sub__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__add__(left->value, right->value);
	return retptr;
}

pyint *pyint__mul__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__mul__(left->value, right->value);
	return retptr;
}

pyint *pyint__div__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__div__(left->value, right->value);
	return retptr;
}

pyint *pyint__mod__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__mod__(left->value, right->value);
	return retptr;
}

pyint *pyint__lshift__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__lshift__(left->value, right->value);
	return retptr;
}

pyint *pyint__rshift__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__rshift__(left->value, right->value);
	return retptr;
}

int pyint__cmp__(pyint *left, pyint *right) {
	return integer__cmp__(left->value, right->value);
}