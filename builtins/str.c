#include <string.h>
#include "../pytype/pystr.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../evaluate.h"

pystr *str_init(pyargument *argument) {
    if (argument->assign_target_list) {
        char *first = ((identifier *)argument->assign_target_list->content)->value;
        if (!strcmp(first, "object")) {
            return str(argument->assign_value_list->content);
        }
    }
    else {
        return str(argument->value_list->content);
    }
}

void *str_capitalize(list *val) {}