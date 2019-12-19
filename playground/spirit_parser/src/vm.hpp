#pragma once

#include <iostream>
#include <vector>

namespace client
{
///////////////////////////////////////////////////////////////////////////
//  The Virtual Machine
///////////////////////////////////////////////////////////////////////////
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
        case op_neg: return out << "op_neg";
        case op_add: return out << "op_add";
        case op_sub: return out << "op_sub";
        case op_mul: return out << "op_mul";
        case op_div: return out << "op_div";
        case op_not: return out << "op_not";
        case op_eq: return out << "op_eq";
        case op_neq: return out << "op_neq";
        case op_lt: return out << "op_lt";
        case op_lte: return out << "op_lte";
        case op_gt: return out << "op_gt";
        case op_gte: return out << "op_gte";
        case op_and: return out << "op_and";
        case op_or: return out << "op_or";
        case op_load: return out << "op_load";
        case op_store: return out << "op_store";
        case op_int: return out << "op_int";
        case op_true: return out << "op_true";
        case op_false: return out << "op_false";
        case op_jump_if: return out << "op_jump_if";
        case op_jump: return out << "op_jump";
        case op_stk_adj: return out << "op_stk_adj";
        case op_call: return out << "op_call";
        case op_return: return out << "op_return";
    }
}

class vmachine
{
public:
    vmachine(unsigned stackSize = 4096) : stack(stackSize) {}

    int execute(std::vector<int> const& code  // the program code
                ,
                std::vector<int>::const_iterator pc  // program counter
                ,
                std::vector<int>::iterator frame_ptr  // start of arguments and locals
    );

    int execute(std::vector<int> const& code) { return execute(code, code.begin(), stack.begin()); };

    std::vector<int> const& get_stack() const { return stack; };

private:
    std::vector<int> stack;
};
}  // namespace client
