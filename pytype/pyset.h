#ifndef PYTYPE_PYSET_H
#define PYTYPE_PYSET_H

#include "../list.h"

typedef struct pyset pyset;
struct pyset {
	int type;
	int ref;
	list *values;
};

#endif /* PYTYPE_PYSET_H */