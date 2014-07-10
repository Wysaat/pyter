#include <string.h>
#include "../pytype/pystr.h"
#include "../pytype/py__builtins__.h"
#include "../pytype/others.h"
#include "../pytype/methods.h"
#include "../evaluate.h"
#include "../types.h"
#include "../__builtins__.h"

void def_func(environment *env, void *func(), char *name);
void def_str_func(void *func(), char *name);

pystr *str_init(pyargument *argument) {
    if (!argument)
        return pystr__init__();
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

void *str_capitalize(pyargument* argument) {
    list *val = argument->value_list;
    return pystr_capitalize(val->content);
}

void def_str_capitalize_func(environment *env) {
    pybuiltin_function *str_capitalize_func = pybuiltin_function__init__("capitalize", str_capitalize);
    store_id(env, "capitalize", str_capitalize_func);
}

void *str_casefold(pyargument *argument) {
    list *val = argument->value_list;
    return pystr_casefold(val->content);
}

void def_str_casefold_func(environment *env) {
    pybuiltin_function *str_casefold_func = pybuiltin_function__init__("casefold", str_casefold);
    store_id(env, "casefold", str_casefold_func);
}

void *str_center(pyargument *argument) {
    list *val = argument->value_list;
    if (!val->next->next)
        return pystr_center(val->content, val->next->content, 0);
    else
        return pystr_center(val->content, val->next->content, val->next->next->content);
}

void def_str_center_func(environment *env) {
    pybuiltin_function *str_center_func = pybuiltin_function__init__("center", str_center);
    store_id(env, "center", str_center_func);
}

void *str_count(pyargument *argument) {
    list *val = argument->value_list;
    if (!val->next->next)
        return pystr_count(val->content, val->next->content, 0, 0);
    else if (!val->next->next->next)
        return pystr_count(val->content, val->next->content, val->next->next->content, 0);
    else
        return pystr_count(val->content, val->next->content, val->next->next->content, val->next->next->next->content);
}

void def_str_count_func(environment *env) {
    pybuiltin_function *str_count_func = pybuiltin_function__init__("count", str_count);
    store_id(env, "count", str_count_func);
}

void *str_endswith(pyargument *argument) {
    int start, end;
    list *val = argument->value_list;
    char *string = ((pystr *)val->content)->value;
    char *suffix = ((pystr *)val->next->content)->value;
    int len = strlen(string), slen = strlen(suffix);
    if (val->next->next) {
        start = pyint_to_int(val->next->next->content);
        if (val->next->next->next) {
            end =  pyint_to_int(val->next->next->next->content);
            if (end < 0)
                end += len;
        }
        else
            end = len;
    }
    else {
        start = 0;
        end = len;
    }
    if (end - start < slen)
        return PYBOOL(0);
    else {
        int i, j;
        for (i = slen-1, j = end-1; i >= 0; i--, j--) {
            if (suffix[i] != string[j])
                return PYBOOL(0);
        }
        return PYBOOL(1);
    }
}

void def_str_endswith_func(environment *env) {
    pybuiltin_function *str_endswith_func = pybuiltin_function__init__("endswith", str_endswith);
    store_id(env, "endswith", str_endswith_func);
}

void *str_expandtabs(pyargument *argument) {
    int tabsize = 8;
    if (argument->value_list->next) {
        tabsize = pyint_to_int(argument->value_list->next->content);
    }
    else if (argument->assign_target_list) {
        char *first = ((identifier *)argument->assign_target_list->content)->value;
        if (!strcmp(first, "tabsize"))
            tabsize = pyint_to_int(argument->assign_target_list->content);
    }
    char *ptr, *string = ((pystr *)argument->value_list->content)->value;
    int nlen = strlen(string);
    for (ptr = string; *ptr; ptr++) {
        if (*ptr == '\t')
            nlen += tabsize-1;
    }
    char nstring[nlen+1], *ptr2 = nstring;
    for (ptr = string; *ptr; ptr++) {
        if (*ptr == '\t') {
            int i;
            for (i = 0; i < tabsize; i++)
                *ptr2++ = ' ';
        }
        else
            *ptr2++ = *ptr;
    }
    nstring[nlen] = 0;
    return pystr_init2(nstring);
}

void def_str_expandtabs(environment *env) {
    pybuiltin_function *str_expandtabs_func = pybuiltin_function__init__("expandtabs", str_expandtabs);
    store_id(env, "expandtabs", str_expandtabs_func);
}

void *str_find(pyargument *argument) {
    char *string = ((pystr *)argument->value_list->content)->value;
    char *subs = ((pystr *)argument->value_list->next->content)->value;
    int len = strlen(string);
    int start = 0, end = len;
    if (argument->value_list->next->next) {
        start = pyint_to_int(argument->value_list->next->next->content);
        if (argument->value_list->next->next->next)
            end = pyint_to_int(argument->value_list->next->next->next->content);
    }
    return int_to_pyint(find_subs(string, subs));
}

void def_func(environment *env, void *func(), char *name) {
    pybuiltin_function *builtin_func = pybuiltin_function__init__(name, func);
    store_id(env, name, builtin_func);
}

void def_str_func(void *func(), char *name) {
    def_func(str_class.env, func, name);
}

void def_str(environment *env) {
    str_class.type = pyclass_t;
    str_class.class = &type_class;
    str_class.id = "str";
    str_class.env = environment_init(0);
    str_class.inheritance = 0;
    def_str_capitalize_func(str_class.env);
    def_str_casefold_func(str_class.env);
    def_str_center_func(str_class.env);
    def_str_count_func(str_class.env);
    def_str_endswith_func(str_class.env);
    def_str_expandtabs(str_class.env);
    def_str_func(str_find, "find");
    store_id(env, "str", &str_class);
}