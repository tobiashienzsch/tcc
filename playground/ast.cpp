#include "ast.hpp"

auto operator<<(std::ostream &out, ASTNodeType const type) -> std::ostream & {
  switch (type) {
  case ASTNodeType::Operand: {
    out << "Operand";
    break;
  }
  case ASTNodeType::ConstantExpression: {
    out << "ConstantExpr";
    break;
  }
  case ASTNodeType::BinaryExpression: {
    out << "BinaryExpr";
    break;
  }
  case ASTNodeType::BracedExpression: {
    out << "BracedExpr";
    break;
  }
  }
  return out;
}