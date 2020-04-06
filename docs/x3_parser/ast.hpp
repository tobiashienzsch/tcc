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

struct Variable : x3::position_tagged {
  Variable(std::string newName = "") : name(std::move(newName)) {}
  std::string name;
};

struct Operand : x3::variant<Nil, uint64_t, Variable, x3::forward_ast<Unary>, x3::forward_ast<Expression>> {
  using base_type::base_type;
  using base_type::operator=;
};

enum class OpToken {
  Plus,
  Minus,
  Times,
  Divide,
  Positive,
  Negative,
  Not,
  Equal,
  NotEqual,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  And,
  Or

};

inline auto operator<<(std::ostream& out, OpToken op) -> std::ostream& {
  switch (op) {
    case OpToken::Plus:
      out << "plus";
      break;
    case OpToken::Minus:
      out << "minus";
      break;
    case OpToken::Times:
      out << "times";
      break;
    case OpToken::Divide:
      out << "divide";
      break;
    case OpToken::Positive:
      out << "positive";
      break;
    case OpToken::Negative:
      out << "negative";
      break;
    case OpToken::Not:
      out << "not";
      break;
    case OpToken::Equal:
      out << "equal";
      break;
    case OpToken::NotEqual:
      out << "not equal";
      break;
    case OpToken::Less:
      out << "less";
      break;
    case OpToken::LessEqual:
      out << "less equal";
      break;
    case OpToken::Greater:
      out << "greater";
      break;
    case OpToken::GreaterEqual:
      out << "greater equal";
      break;
    case OpToken::And:
      out << "and";
      break;
    case OpToken::Or:
      out << "or";
      break;
  }
  return out;
}

struct Unary {
  OpToken operator_;
  Operand operand_;
};

struct Operation : x3::position_tagged {
  OpToken operator_;
  Operand operand_;
};

struct Expression : x3::position_tagged {
  Operand first;
  std::list<Operation> rest;
};

struct Assignment : x3::position_tagged {
  Variable lhs;
  Expression rhs;
};

struct VariableDeclaration {
  Assignment assign;
};

struct ReturnStatement : x3::position_tagged {
  Expression expression;
};

struct IfStatement;
struct WhileStatement;
struct StatementList;
struct CompoundStatement;

struct Statement : x3::variant<VariableDeclaration, Assignment, ReturnStatement, boost::recursive_wrapper<IfStatement>,
                               boost::recursive_wrapper<WhileStatement>, boost::recursive_wrapper<StatementList>,
                               boost::recursive_wrapper<CompoundStatement>> {
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

struct CompoundStatement {
  Statement body;
};

// print functions for debugging
inline auto operator<<(std::ostream& out, Nil) -> std::ostream& {
  out << "Nil";
  return out;
}

inline auto operator<<(std::ostream& out, Variable const& var) -> std::ostream& {
  out << var.name;
  return out;
}
}  // namespace ast
}  // namespace tcc