#ifndef PYTYPE_PYDICT_H
#define PYTYPE_PYDICT_H

#include "../list.h"

typedef struct pydict pydict;
struct pydict {
	int type;
	int ref;
	list *keys;
	list *values;
};

void *pydict__getitem__(void *left, void *right);
void pydict__setitem__(void *left, void *right, void *value);

#endif /* PYTYPE_PYDICT_H */