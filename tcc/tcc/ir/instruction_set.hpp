#pragma once

#include <iostream>
#include <vector>

#include "fmt/format.h"

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

    Load,   //  load a variable
    Store,  //  store a variable

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

inline std::ostream& operator<<(std::ostream& out, IRByteCode const data)
{
    switch (data)
    {
        case IRByteCode::Negate: return out << "neg";
        case IRByteCode::Addition: return out << "add";
        case IRByteCode::Subtraction: return out << "sub";
        case IRByteCode::Multiplication: return out << "mul";
        case IRByteCode::Division: return out << "div";
        case IRByteCode::Not: return out << "not";
        case IRByteCode::Equal: return out << "eq";
        case IRByteCode::NotEqual: return out << "neq";
        case IRByteCode::Less: return out << "lt";
        case IRByteCode::LessEqual: return out << "lte";
        case IRByteCode::Greater: return out << "gt";
        case IRByteCode::GreaterEqual: return out << "gte";
        case IRByteCode::And: return out << "and";
        case IRByteCode::Or: return out << "or";
        case IRByteCode::Load: return out << "load";
        case IRByteCode::Store: return out << "store";
        case IRByteCode::Int: return out << "int";
        case IRByteCode::True: return out << "true";
        case IRByteCode::False: return out << "false";
        case IRByteCode::JumpIf: return out << "jump_if";
        case IRByteCode::Jump: return out << "jump";
        case IRByteCode::StackAdjust: return out << "stk_adj";
        case IRByteCode::Call: return out << "call";
        case IRByteCode::Return: return out << "return";
    }

    return out;
}

}  // namespace tcc
