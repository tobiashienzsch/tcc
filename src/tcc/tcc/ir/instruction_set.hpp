#pragma once

#include <iostream>

namespace tcc
{
enum class IRByteCode
{
    Negate,          //  negate the top stack entry
    Addition,        //  add top two stack entries
    Subtraction,     //  subtract top two stack entries
    Multiplication,  //  multiply top two stack entries
    Division,        //  divide top two stack entries

    Not,           //  boolean negate the top stack entry
    Equal,         //  compare the top two stack entries for ==
    NotEqual,      //  compare the top two stack entries for !=
    Less,          //  compare the top two stack entries for <
    LessEqual,     //  compare the top two stack entries for <=
    Greater,       //  compare the top two stack entries for >
    GreaterEqual,  //  compare the top two stack entries for >=

    And,  //  logical and top two stack entries
    Or,   //  logical or top two stack entries

    Load,      //  load a variable
    Store,     //  store a variable
    ArgLoad,   // load function argument
    ArgStore,  // store function argument

    Int,    //  push constant integer into the stack
    True,   //  push constant 0 into the stack
    False,  //  push constant 1 into the stack

    JumpIf,  //  jump to a relative position in the code if top stack
             //  evaluates to false
    Jump,    //  jump to a relative position in the code

    StackAdjust,  // adjust the stack (for args and locals)
    Call,         // function call
    Return        // return from function
};

auto operator<<(std::ostream& out, IRByteCode data) -> std::ostream&;
}  // namespace tcc
