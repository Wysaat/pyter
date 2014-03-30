#include "../list.h"

typedef struct pydict pydict;
struct pydict {
	int type;
	list *keys;
	list *values;
};