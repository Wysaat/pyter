    while ((unsigned char)sc_curch(sc) == 27) {
        sc_readch(sc);
        if ((unsigned char)sc_curch(sc) == 91) {
            sc_readch(sc);
            if ((unsigned char)sc_curch(sc) == 65) {
                sc_readch(sc);
                sc->line_ptr = sc->line_ptr->prev;
            }
            else if ((unsigned char)sc_curch(sc) == 66) {
                sc_readch(sc);
                sc->line_ptr = sc->line_ptr->next;
            }
            sc->line = sc->line_ptr->content;
            sc->line[strlen(sc->line)-1] = 0;
            sc->ln++;
            sc->ind = strlen(sc->line);
            printf("%s", sc->line);
        }
    }

void del(void *vptr) {
    if (!vptr)
        return;
    if (type(vptr) == identifier_t)
        identifier_del(vptr);
    else if (type(vptr) == int_expr_t)
        int_expr_del(vptr);
    else if (type(vptr) == bool_expr_t)
        bool_expr_del(vptr);
    else if (type(vptr) == float_expr_t)
        float_expr_del(vptr);
    else if (type(vptr) == imag_expr_t)
        imag_expr_del(vptr);
    else if (type(vptr) == str_expr_t)
        str_expr_del(vptr);
    else if (type(vptr) == parenth_form_t)
        parenth_form_del(vptr);
    else if (type(vptr) == generator_t)
        generator_del(vptr);
    else if (type(vptr) == list_expr_t)
        list_expr_del(vptr);
    else if (type(vptr) == set_expr_t)
        set_expr_del(vptr);
    else if (type(vptr) == dict_expr_t)
        dict_expr_del(vptr);
    else if (type(vptr) == list_comprehension_t)
        list_comprehension_del(vptr);
    else if (type(vptr) == yield_atom_t)
        yield_atom_del(vptr);
    else if (type(vptr) == attributeref_t)
        attributeref_del(vptr);
    else if (type(vptr) == slice_expr_t)
        slice_expr_del(vptr);
    else if (type(vptr) == slicing_t)
        slicing_del(vptr);
    else if (type(vptr) == subsc_expr_t)
        subsc_expr_del(vptr);
    else if (type(vptr) == subscription_t)
        subscription_del(vptr);
    else if (type(vptr) == call_t)
        call_del(vptr);
    else if (type(vptr) == power_t)
        power_del(vptr);
    else if (type(vptr) == u_expr_t)
        u_expr_del(vptr);
    else if (type(vptr) == b_expr_t)
        b_expr_del(vptr);
    else if (type(vptr) == not_test_t)
        not_test_del(vptr);
    else if (type(vptr) == comparison_t)
        comparison_del(vptr);
    else if (type(vptr) == conditional_expression_t)
        conditional_expression_del(vptr);
    else if (type(vptr) == lambda_expr_t)
        lambda_expr_del(vptr);
    else if (type(vptr) == expression_list_t)
        expression_list_del(vptr);

    else if (type(vptr) == expression_stmt_t)
        expression_stmt_del(vptr);
    else if (type(vptr) == assignment_stmt_t)
        assignment_stmt_del(vptr);
    else if (type(vptr) == return_stmt_t)
        return_stmt_del(vptr);
    else if (type(vptr) == yield_stmt_t)
        yield_stmt_del(vptr);
    else if (type(vptr) == break_stmt_t)
        break_stmt_del(vptr);
    else if (type(vptr) == continue_stmt_t)
        continue_stmt_del(vptr);
    else if (type(vptr) == pass_stmt_t)
        pass_stmt_del(vptr);
    else if (type(vptr) == import_stmt_t)
        import_stmt_del(vptr);
    else if (type(vptr) == del_stmt_t)
        del_stmt_del(vptr);
    else if (type(vptr) == stmt_list_t)
        stmt_list_del(vptr);
    else if (type(vptr) == if_stmt_t)
        if_stmt_del(vptr);
    else if (type(vptr) == while_stmt_t)
        while_stmt_del(vptr);
    else if(type(vptr) == for_stmt_t)
        for_stmt_del(vptr);
    else if (type(vptr) == funcdef_t)
        funcdef_del(vptr);
    else if (type(vptr) == classdef_t)
        classdef_del(vptr);
    else if (type(vptr) == suite_t)
        suite_del(vptr);

    else if (type(vptr) == __pyargument_t)
        pyargument_del(vptr);

    else if (type(vptr) == environment_t)
        environment_del(vptr);

    else if (type(vptr) == pyint_t)
        pyint_del(vptr);
    else if (type(vptr) == pybool_t)
        pybool_del(vptr);
    else if (type(vptr) == pyfloat_t)
        pyfloat_del(vptr);
    else if (type(vptr) == pycomplex_t)
        pycomplex_del(vptr);
    else if (type(vptr) == pystr_t)
        pystr_del(vptr);
    else if (type(vptr) == pylist_t)
        pylist_del(vptr);
    else if (type(vptr) == pytuple_t)
        pytuple_del(vptr);
    else if (type(vptr) == pyset_t)
        pyset_del(vptr);
    else if (type(vptr) == pydict_t)
        pydict_del(vptr);
    else if (type(vptr) == pyfunction_t)
        pyfunction_del(vptr);
    else if (type(vptr) == pyclass_t)
        pyclass_del(vptr);
    else if (type(vptr) == pymodule_t)
        pymodule_del(vptr);
    else if (type(vptr) == pyNone_t)
        pyNone_del(vptr);
    else if (type(vptr) == pyrange_t)
        pyrange_del(vptr);
    else if (type(vptr) == pybuiltin_function_t)
        pybuiltin_function_del(vptr);
    else if (type(vptr) == instance_t)
        instance_del(vptr);
}

void ref(void *vptr) {
    if (!vptr)
        return;
    if (type(vptr) == pyint_t)
        ++((pyint *)vptr)->ref;
    else if (type(vptr) == pybool_t)
        ++((pybool *)vptr)->ref;
    else if (type(vptr) == pyfloat_t)
        ++((pyfloat *)vptr)->ref;
    else if (type(vptr) == pycomplex_t)
        ++((pycomplex *)vptr)->ref;
    else if (type(vptr) == pystr_t)
        ++((pystr *)vptr)->ref;
    else if (type(vptr) == pylist_t)
        ++((pylist *)vptr)->ref;
    else if (type(vptr) == pytuple_t)
        ++((pytuple *)vptr)->ref;
    else if (type(vptr) == pyset_t)
        ++((pyset *)vptr)->ref;
    else if (type(vptr) == pydict_t)
        ++((pydict *)vptr)->ref;
    else if (type(vptr) == pyfunction_t)
        ++((pyfunction *)vptr)->ref;
    else if (type(vptr) == pyclass_t)
        ++((pyclass *)vptr)->ref;
    else if (type(vptr) == pymodule_t)
        ++((pymodule *)vptr)->ref;
    else if (type(vptr) == pyNone_t)
        ++((pyNone *)vptr)->ref;
    else if (type(vptr) == pyrange_t)
        ++((pyrange *)vptr)->ref;
    else if (type(vptr) == pybuiltin_function_t)
        ++((pybuiltin_function *)vptr)->ref;
    else if (type(vptr) == instance_t)
        ++((instance *)vptr)->ref;
}

int nref(void *vptr) {
    if (!vptr)
        return;
    if (type(vptr) == pyint_t)
        return --((pyint *)vptr)->ref;
    else if (type(vptr) == pybool_t)
        return --((pybool *)vptr)->ref;
    else if (type(vptr) == pyfloat_t)
        return --((pyfloat *)vptr)->ref;
    else if (type(vptr) == pycomplex_t)
        return --((pycomplex *)vptr)->ref;
    else if (type(vptr) == pystr_t)
        return --((pystr *)vptr)->ref;
    else if (type(vptr) == pylist_t)
        return --((pylist *)vptr)->ref;
    else if (type(vptr) == pytuple_t)
        return --((pytuple *)vptr)->ref;
    else if (type(vptr) == pyset_t)
        return --((pyset *)vptr)->ref;
    else if (type(vptr) == pydict_t)
        return --((pydict *)vptr)->ref;
    else if (type(vptr) == pyfunction_t)
        return --((pyfunction *)vptr)->ref;
    else if (type(vptr) == pyclass_t)
        return --((pyclass *)vptr)->ref;
    else if (type(vptr) == pymodule_t)
        return --((pymodule *)vptr)->ref;
    else if (type(vptr) == pyNone_t)
        return --((pyNone *)vptr)->ref;
    else if (type(vptr) == pyrange_t)
        return --((pyrange *)vptr)->ref;
    else if (type(vptr) == pybuiltin_function_t)
        return --((pybuiltin_function *)vptr)->ref;
    else if (type(vptr) == instance_t)
        return --((instance *)vptr)->ref;
}

