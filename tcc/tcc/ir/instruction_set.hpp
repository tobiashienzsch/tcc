#pragma once

#include <iostream>
#include <vector>

#include "fmt/format.h"

namespace tcc
{
enum class IRByteCode
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

inline std::ostream& operator<<(std::ostream& out, IRByteCode const data)
{
    switch (data)
    {
        case IRByteCode::op_neg: return out << "neg";
        case IRByteCode::op_add: return out << "add";
        case IRByteCode::op_sub: return out << "sub";
        case IRByteCode::op_mul: return out << "mul";
        case IRByteCode::op_div: return out << "div";
        case IRByteCode::op_not: return out << "not";
        case IRByteCode::op_eq: return out << "eq";
        case IRByteCode::op_neq: return out << "neq";
        case IRByteCode::op_lt: return out << "lt";
        case IRByteCode::op_lte: return out << "lte";
        case IRByteCode::op_gt: return out << "gt";
        case IRByteCode::op_gte: return out << "gte";
        case IRByteCode::op_and: return out << "and";
        case IRByteCode::op_or: return out << "or";
        case IRByteCode::op_load: return out << "load";
        case IRByteCode::op_store: return out << "store";
        case IRByteCode::op_int: return out << "int";
        case IRByteCode::op_true: return out << "true";
        case IRByteCode::op_false: return out << "false";
        case IRByteCode::op_jump_if: return out << "jump_if";
        case IRByteCode::op_jump: return out << "jump";
        case IRByteCode::op_stk_adj: return out << "stk_adj";
        case IRByteCode::op_call: return out << "call";
        case IRByteCode::op_return: return out << "return";
    }

    return out;
}

}  // namespace tcc
