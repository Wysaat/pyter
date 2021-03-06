#ifndef TYPES_H
#define TYPES_H

enum types { identifier_t = 0,
             int_expr_t = 1,
             bool_expr_t = 2,
             float_expr_t = 3,
             imag_expr_t = 4,
             str_expr_t = 5,
             parenth_form_t = 6,
             list_expr_t = 7,
             set_expr_t = 8,
             dict_expr_t = 9,
             list_comprehension_t = 10,
             yield_atom_t = 11,
             generator_t = 12,
             attributeref_t = 13,
             slice_expr_t = 14,
             subsc_expr_t = 15,
             slicing_t = 16,
             subscription_t = 17,
             call_t = 18,
             power_t = 19,
             u_expr_t = 20,
             b_expr_t = 21,
             comparison_t = 22,
             not_test_t = 23,
             conditional_expression_t = 24,
             lambda_expr_t = 25,
             expression_list_t = 26,
             expression_stmt_t = 27,
             assignment_stmt_t = 28,
             return_stmt_t = 29,
             yield_stmt_t = 30,
             break_stmt_t = 31,
             continue_stmt_t = 32,
             pass_stmt_t = 33,
             import_stmt_t = 34,
             import_stmt2_t = 35,
             del_stmt_t = 36,
             stmt_list_t = 37,
             if_stmt_t = 38,
             while_stmt_t = 39,
             for_stmt_t = 40,
             funcdef_t = 41,
             classdef_t = 42,
             suite_t = 43,
             pyint_t = 44,
             pyfloat_t = 45,
             pycomplex_t = 46,
             pystr_t = 47,
             pybool_t = 48,
             pytuple_t = 49,
             pylist_t = 50,
             pyset_t = 51,
             pydict_t = 52,
             pyslice_t = 53,
             pyNone_t = 54,
             pyfunction_t = 55,
             pyclass_t = 56,
             pybuiltin_function_t = 57,
             pymodule_t = 58,
             pygenerator_t = 59,
             list_t = 60,
             instance_t = 61,
             pyrange_t = 62,
             environment_t = 63,
             val_dict_entry_t = 64,
             __pyargument_t = 65,
            };

#endif /* TYPES_H */