#include "ast.hpp"

auto operator<<(std::ostream &out, ASTNodeType const type) -> std::ostream & {
  switch (type) {
  case ASTNodeType::Operator: {
    out << "Operator";
    break;
  }
  case ASTNodeType::Constant: {
    out << "Constant";
    break;
  }
  case ASTNodeType::BinaryExpression: {
    out << "BinaryExpr";
    break;
  }
  case ASTNodeType::UnaryExpression: {
    out << "UnaryExpr";
    break;
  }
  case ASTNodeType::BracedExpression: {
    out << "BracedExpr";
    break;
  }
  }
  return out;
}