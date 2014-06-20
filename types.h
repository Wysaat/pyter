#ifndef TYPES_H
#define TYPES_H

enum types { identifier_t = 0,
             int_expr_t = 1,
             float_expr_t = 2,
             imag_expr_t = 3,
             str_expr_t = 4,
             parenth_form_t = 5,
             list_expr_t = 6,
             set_expr_t = 7,
             dict_expr_t = 8,
             list_comprehension_t = 9,
             yield_atom_t = 10,
             generator_t = 11,
             attributeref_t = 12,
             slice_expr_t = 13,
             subsc_expr_t = 14,
             slicing_t,
             subscription_t,
             call_t,
             power_t,
             u_expr_t,
             b_expr_t,
             comparison_t,
             not_test_t,
             conditional_expression_t,
             lambda_expr_t,
             expression_list_t,
             expression_stmt_t,
             assignment_stmt_t,
             return_stmt_t,
             yield_stmt_t,
             break_stmt_t,
             continue_stmt_t,
             pass_stmt_t,
             import_stmt_t,
             import_stmt2_t,
             del_stmt_t,
             stmt_list_t,
             if_stmt_t,
             while_stmt_t,
             for_stmt_t, funcdef_t,
             classdef_t, suite_t,
             pyint_t,
             pyfloat_t,
             pycomplex_t,
             pystr_t,
             pybool_t,
             pytuple_t,
             pylist_t,
             pyset_t,
             pydict_t,
             pyslice_t,
             pyNone_t,
             pyfunction_t,
             pyclass_t,
             pybuiltin_function_t,
             pymodule_t,
             pygenerator_t,
             list_t,
             instance_t,
             pyrange_t,
             environment_t,
             val_dict_entry_t,
             __pyargument_t,
            };

#endif /* TYPES_H */