#include "../pytype/pyint.h"
#include "../pytype/pyfloat.h"
#include "../pytype/py__builtins__.h"
#include "../pytype/others.h"
#include "../types.h"
#include "../__builtins__.h"


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
    if (argument->assign_target_list) {
    }
    else {
        void *vptr = argument->value_list->content;

        pyint *base = int_to_pyint(10);
        pyint *order = int_to_pyint(1);
        pyint *retptr = int_to_pyint(0);
        pyint *val;

        char *string, *ptr;
        switch (type(vptr)) {
            case pyint_t:
                return vptr;
            case pyfloat_t:
                return pyfloat__int__(vptr);
            case pystr_t:
                if (argument->value_list->next)
                    base = argument->value_list->next->content;
                string = ((pystr *)vptr)->value;
                ptr = string;
                while (*ptr)
                    ptr++;
                ptr--;
                for ( ; ptr != string-1; ptr--) {
                    if (*ptr >= '0' && *ptr <= '9')
                        val = pyint_mul2(int_to_pyint(*ptr - '0'), order);
                    else if (*ptr >= 'a' && *ptr <= 'z')
                        val = pyint_mul2(int_to_pyint(*ptr - 'a' + 10), order);
                    else if (*ptr >= 'A' && *ptr <= 'Z')
                        val = pyint_mul2(int_to_pyint(*ptr - 'A' + 10), order);
                    order = pyint_mul2(order, base);
                    retptr = pyint_add3(retptr, val);
                }
                return retptr;
        }
    }
}

void def_func(environment *env, void *func(), char *name);

void def_int_func(void *func(), char *name) {
    def_func(int_class.env, func, name);
}

void def_int(environment *env) {
    int_class.type = pyclass_t;
    int_class.ref = 0;
    int_class.class = &type_class;
    int_class.id = "int";
    int_class.env = environment_init(0);
    int_class.inheritance = 0;
    store_id(env, "int", &int_class);
}
