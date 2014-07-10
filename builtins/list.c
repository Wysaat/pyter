#include "../pytype/pylist.h"
#include "../pytype/pyclass.h"
#include "../pytype/others.h"
#include "../list.h"
#include "../environment.h"
#include "../__builtins__.h"
#include "../types.h"

// /* CAUTION: nested function definition is not standard C, change it someday.. */
// void *def_sort_func_of_list(environment *env) {
//     int comp(void *left, void *right) {
//         if (type(left) == pyint_t && type(right) == pyint_t) {
//             if (is_true(pyint__lt__((pyint *)left, (pyint *)right)))
//                 return -1;
//             else if (is_true(pyint__eq__((pyint *)left, (pyint *)right)))
//                 return 0;
//             else
//                 return 1;
//         }
//     }
//     void *sort_of_list(pyargument *argument) {
//         list *val = argument->value_list;
//         pylist__sort__((pylist *)val->content, comp);
//         return pyNone_init();
//     }
//     pybuiltin_function *sort_func_of_list = pybuiltin_function__init__("sort", sort_of_list);
//     store_id(env, "sort", sort_func_of_list);
// }

void *_append(pyargument *argument) {
    list *val = argument->value_list;
    pylist__append__(val->content, val->next->content);
    return pyNone_init();
}

void def_list_func(void *func(), char *name) {
    def_func(list_class.env, func, name);
}

void def_list(environment *env) {
    list_class.type = pyclass_t;
    list_class.class = &type_class;
    list_class.id = "list";
    list_class.env = environment_init(0);
    list_class.inheritance = 0;
    def_list_func(_append, "append");
    store_id(env, "list", &list_class);
}
