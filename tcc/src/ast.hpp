#pragma once

#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <list>

namespace tcc {
namespace ast {
namespace x3 = boost::spirit::x3;

struct Nil {};
struct Unary;
struct Expression;

struct variable : x3::position_tagged {
  variable(std::string newName = "") : name(std::move(newName)) {}
  std::string name;
};

struct operand : x3::variant<Nil, uint64_t, variable, x3::forward_ast<Unary>, x3::forward_ast<Expression>> {
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

struct Unary {
  optoken operator_;
  operand operand_;
};

struct operation : x3::position_tagged {
  optoken operator_;
  operand operand_;
};

struct Expression : x3::position_tagged {
  operand first;
  std::list<operation> rest;
};

struct assignment : x3::position_tagged {
  variable lhs;
  Expression rhs;
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
  Expression condition;
  Statement then;
  boost::optional<Statement> else_;
};

struct WhileStatement {
  Expression condition;
  Statement body;
};

// print functions for debugging
inline auto operator<<(std::ostream& out, Nil) -> std::ostream& {
  out << "Nil";
  return out;
}

inline auto operator<<(std::ostream& out, variable const& var) -> std::ostream& {
  out << var.name;
  return out;
}
}  // namespace ast
}  // namespace tcc
