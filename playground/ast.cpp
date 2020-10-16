#include "ast.hpp"

std::ostream& operator<<(std::ostream& out, ASTNodeType const type)
{
    switch (type)
    {
        case ASTNodeType::Operand:
        {
            out << "Operand";
            break;
        }
        case ASTNodeType::ConstantExpression:
        {
            out << "ConstantExpr";
            break;
        }
        case ASTNodeType::BinaryExpression:
        {
            out << "BinaryExpr";
            break;
        }
    }
    return out;
}