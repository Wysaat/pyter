#ifndef PYTYPE_PYDICT_H
#define PYTYPE_PYDICT_H

#include "../list.h"

typedef struct pydict pydict;
struct pydict {
	int type;
	list *keys;
	list *values;
};

#endif /* PYTYPE_PYDICT_H */