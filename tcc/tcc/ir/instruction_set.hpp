#pragma once

#include <iostream>
#include <vector>

#include "fmt/format.h"

namespace tcc
{
enum byte_code
{
    op_neg,  //  negate the top stack entry
    op_add,  //  add top two stack entries
    op_sub,  //  subtract top two stack entries
    op_mul,  //  multiply top two stack entries
    op_div,  //  divide top two stack entries

    op_not,  //  boolean negate the top stack entry
    op_eq,   //  compare the top two stack entries for ==
    op_neq,  //  compare the top two stack entries for !=
    op_lt,   //  compare the top two stack entries for <
    op_lte,  //  compare the top two stack entries for <=
    op_gt,   //  compare the top two stack entries for >
    op_gte,  //  compare the top two stack entries for >=

    op_and,  //  logical and top two stack entries
    op_or,   //  logical or top two stack entries

    op_load,   //  load a variable
    op_store,  //  store a variable

    op_int,    //  push constant integer into the stack
    op_true,   //  push constant 0 into the stack
    op_false,  //  push constant 1 into the stack

    op_jump_if,  //  jump to a relative position in the code if top stack
                 //  evaluates to false
    op_jump,     //  jump to a relative position in the code

    op_stk_adj,  // adjust the stack (for args and locals)
    op_call,     // function call
    op_return    // return from function
};

inline std::ostream& operator<<(std::ostream& out, byte_code const data)
{
    switch (data)
    {
        case op_neg: return out << "neg";
        case op_add: return out << "add";
        case op_sub: return out << "sub";
        case op_mul: return out << "mul";
        case op_div: return out << "div";
        case op_not: return out << "not";
        case op_eq: return out << "eq";
        case op_neq: return out << "neq";
        case op_lt: return out << "lt";
        case op_lte: return out << "lte";
        case op_gt: return out << "gt";
        case op_gte: return out << "gte";
        case op_and: return out << "and";
        case op_or: return out << "or";
        case op_load: return out << "load";
        case op_store: return out << "store";
        case op_int: return out << "int";
        case op_true: return out << "true";
        case op_false: return out << "false";
        case op_jump_if: return out << "jump_if";
        case op_jump: return out << "jump";
        case op_stk_adj: return out << "stk_adj";
        case op_call: return out << "call";
        case op_return: return out << "return";
    }

    return out;
}

}  // namespace tcc
