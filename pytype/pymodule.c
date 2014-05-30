#include "pymodule.h"
#include "../types.h"
#include "../environment.h"
#include <string.h>
#include <stdlib.h>

pymodule *pymodule_init(char *name) {
	pymodule *retptr = (pymodule *)malloc(sizeof(pymodule));
	memset(retptr, 0, sizeof(pymodule));
	retptr->type = pymodule_t;
	retptr->name = strdup(name);
	retptr->env = environment_init(0);
	return retptr;
}