#include "../pytype/pyint.h"
#include "../pytype/pyfloat.h"
#include "../pytype/py__builtins__.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../types.h"
#include "../__builtins__.h"
#include "../evaluate.h"


/* from the Python 3.4 doc:
 *
 *     If x is not a number or if base is given, then x must be a string, bytes, 
 *     or bytearray instance representing an integer literal in radix base. 
 *     Optionally, the literal can be preceded by + or - (with no space in between) 
 *     and surrounded by whitespace. A base-n literal consists of the digits 0 
 *     to n-1, with a to z (or A to Z) having values 10 to 35. The default base is
 *     10. The allowed values are 0 and 2-36. Base-2, -8, and -16 literals can be
 *     optionally prefixed with 0b/0B, 0o/0O, or 0x/0X, as with integer literals
 *     in code. Base 0 means to interpret exactly as a code literal, so that the 
 *     actual base is 2, 8, 10, or 16, and so that int('010', 0) is not legal,
 *     while int('010') is, as well as int('010', 8).
 */

pyint *int_init(pyargument *argument) {
    if (!argument)
        return int_to_pyint(0);

    void *x;
    pyint *base;

    if (argument->value_list) {
        x = argument->value_list->content;
        if (argument->value_list->next)
            base = argument->value_list->next->content;
        else if (argument->assign_target_list) {
            char *id1 = ((identifier *)argument->assign_target_list->content)->value;
            if (!strcmp(id1, "base"))
                base = argument->assign_value_list->content;
        }
        else
            base = int_to_pyint(10);
    }
    else {
        if (argument->assign_target_list) {
            char *id1 = ((identifier *)argument->assign_target_list->content)->value;
            if (!strcmp(id1, "base")) {
                base = argument->assign_value_list->content;
                if (argument->assign_target_list->next) {
                    char *id2 = ((identifier *)argument->assign_target_list->next->content)->value;
                    if (!strcmp(id2, "x"))
                        x = argument->assign_value_list->next->content;
                }
            }
            else if (!strcmp(id1, "x")) {
                x = argument->assign_value_list->content;
                if (argument->assign_target_list->next) {
                    char *id2 = ((identifier *)argument->assign_target_list->next->content)->value;
                    if (!strcmp(id2, "base"))
                        base = argument->assign_value_list->next->content;
                }
                else
                    base = int_to_pyint(10);
            }
        }
    }

    if (type(x) == instance_t) {
        pyclass *class = ((instance *)x)->class;
        return __call__(env_find(class->env, "__int__"), argument);
    }

    return pyint_init2(x, base);
}

void def_func(environment *env, void *func(), char *name);

void def_int_func(void *func(), char *name) {
    def_func(int_class.env, func, name);
}

void def_int(environment *env) {
    int_class.type = pyclass_t;
    int_class.class = &type_class;
    int_class.id = "int";
    int_class.env = environment_init(0);
    int_class.inheritance = 0;
    store_id(env, "int", &int_class);
}
