#pragma once

#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <list>

namespace tcc {
namespace ast {
namespace x3 = boost::spirit::x3;

struct nil {};
struct unary;
struct expression;

struct variable : x3::position_tagged {
  variable(std::string newName = "") : name(std::move(newName)) {}
  std::string name;
};

struct operand : x3::variant<nil, uint64_t, variable, x3::forward_ast<unary>, x3::forward_ast<expression>> {
  using base_type::base_type;
  using base_type::operator=;
};

enum optoken {
  op_plus,
  op_minus,
  op_times,
  op_divide,
  op_positive,
  op_negative,
  op_not,
  op_equal,
  op_not_equal,
  op_less,
  op_less_equal,
  op_greater,
  op_greater_equal,
  op_and,
  op_or

};

struct unary {
  optoken operator_;
  operand operand_;
};

struct operation : x3::position_tagged {
  optoken operator_;
  operand operand_;
};

struct expression : x3::position_tagged {
  operand first;
  std::list<operation> rest;
};

struct assignment : x3::position_tagged {
  variable lhs;
  expression rhs;
};

struct variable_declaration {
  assignment assign;
};

struct IfStatement;
struct WhileStatement;
struct StatementList;

struct Statement : x3::variant<variable_declaration, assignment, boost::recursive_wrapper<IfStatement>,
                               boost::recursive_wrapper<WhileStatement>, boost::recursive_wrapper<StatementList>> {
  using base_type::base_type;
  using base_type::operator=;
};

struct StatementList : std::list<Statement> {};

struct IfStatement {
  expression condition;
  Statement then;
  boost::optional<Statement> else_;
};

struct WhileStatement {
  expression condition;
  Statement body;
};

// print functions for debugging
inline auto operator<<(std::ostream& out, nil) -> std::ostream& {
  out << "nil";
  return out;
}

inline auto operator<<(std::ostream& out, variable const& var) -> std::ostream& {
  out << var.name;
  return out;
}
}  // namespace ast
}  // namespace tcc
