#include "../cpyter.h"

pyint *pyint__init__() {
    pyint *retptr = (pyint *)malloc(sizeof(pyint));
    retptr->type = pyint_t;
    return retptr;
}

void pyint__del__(pyint *val) {
	integer__del__(val->value);
	free(val);
}

pyint *pyint__add__(pyint *left, pyint *right) {
    pyint *retptr = pyint__init__();
    retptr->value = integer__add__(left->value, right->value);
    return retptr;
}

pyint *pyint__sub__(pyint *left, pyint *right) {
	pyint *retptr = pyint__init__();
	retptr->value = integer__sub__(left->value, right->value);
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

pybool *pyint__lt__(pyint *left, pyint *right) {
	return PYBOOL(integer__lt__(left, right));
}

pybool *pyint__gt__(pyint *left, pyint *right) {
	return PYBOOL(integer__gt__(left, right));
}

int pyint__cmp__(pyint *left, pyint *right) {
	return integer__cmp__(left->value, right->value);
}



void pyint__dec__(pyint *val) {
    integer *new_value = integer__dec__(val->value);
	integer__del__(val->value);
	val->value = new_value;
}

void pyint__inc__(pyint *val) {
	integer *new_value = integer__inc__(val->value);
	integer__del__(val->value);
	val->value = new_value;
}