#ifndef TYPES_H
#define TYPES_H

enum types { identifier_t, int_expr_t, float_expr_t, imag_expr_t, str_expr_t,
             parenth_form_t, list_expr_t, set_expr_t, dict_expr_t,
             list_comprehension_t, yield_atom_t, generator_t,
             attributeref_t, slice_expr_t, subsc_expr_t, slicing_t, subscription_t, call_t,
             power_t, u_expr_t, b_expr_t, comparison_t,
             not_test_t, conditional_expression_t, lambda_expr_t,
             expression_list_t,
             expression_stmt_t, assignment_stmt_t, return_stmt_t, yield_stmt_t,
             break_stmt_t, continue_stmt_t, pass_stmt_t, import_stmt_t, del_stmt_t,
             stmt_list_t,
             if_stmt_t, while_stmt_t, for_stmt_t, funcdef_t, classdef_t, suite_t,
             pyint_t, pyfloat_t, pycomplex_t, pystr_t, pybool_t,
             pytuple_t, pylist_t, pyset_t, pydict_t,
             pyslice_t, pyNone_t,
             pyfunction_t, pyclass_t,
             pybuiltin_function_t, pymodule_t,
             pygenerator_t,
             list_t,
             instance_t,
             pyrange_t,
             environment_t, val_dict_entry_t, };

#endif /* TYPES_H */