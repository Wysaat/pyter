#include "pytype/py__builtins__.h"
#include "evaluate.h"
#include "environment.h"
#include <stdio.h>

void *_print(void *val) {
	if (val)
		print(val);
	else
	    printf("\n");
	return 0;
}

void def_print(environment *env) {
    pybuiltin_function *print_func = pybuiltin_function__init__("print", _print);
    store(env, IDENTIFIER("print"), print_func);
}