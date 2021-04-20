#include "ast.hpp"

auto operator<<(std::ostream &out, ast_node_type const type) -> std::ostream & {
  switch (type) {
  case ast_node_type::op: {
    out << "Operator";
    break;
  }
  case ast_node_type::constant: {
    out << "Constant";
    break;
  }
  case ast_node_type::binary_expression: {
    out << "BinaryExpr";
    break;
  }
  case ast_node_type::unary_expression: {
    out << "UnaryExpr";
    break;
  }
  case ast_node_type::braced_expression: {
    out << "BracedExpr";
    break;
  }
  }
  return out;
}