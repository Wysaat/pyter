#include "../list.h"

typedef struct pyset pyset;
struct pyset {
	int type;
	list *values;
};