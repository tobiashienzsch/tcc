#include "tcc/ir/instruction_set.hpp"

namespace tcc
{
auto operator<<(std::ostream& out, IRByteCode const data) -> std::ostream&
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
        case IRByteCode::ArgLoad: return out << "aload";
        case IRByteCode::ArgStore: return out << "astore";
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