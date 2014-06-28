#include <string.h>
#include "pyset.h"
#include "pyint.h"
#include "pystr.h"
#include "methods.h"
#include "../string.h"

pyint *pyset__len__(void *vptr) {
    pyset *ptr = (pyset *)vptr;
    return int_to_pyint(list_len(ptr->values));
}

pystr *pyset_str(void *vptr) {
    pyset *sptr = (pyset *)vptr;
    buffer2 *buff = buff2_init();
    buff2_add(buff, strdup("{"));
    list *ptr;
    for (ptr = sptr->values; ptr->next; ptr = ptr->next) {
        buff2_add(buff, str(ptr->content)->value);
        buff2_add(buff, strdup(", "));
    }
    buff2_add(buff, str(ptr->content)->value);
    buff2_add(buff, strdup("}"));
    return pystr_init3(buff2_puts2(buff));
}