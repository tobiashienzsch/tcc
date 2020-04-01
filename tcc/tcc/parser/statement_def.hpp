#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"
#include "statement.hpp"

namespace tcc {
namespace parser {
using x3::lexeme;
using x3::raw;
using namespace x3::ascii;

struct StatementList_class;
struct VariableDeclaration_class;
struct Assignment_class;
struct Variable_class;

using StatementList_type = x3::rule<StatementList_class, ast::StatementList>;
using VariableDeclaration_type = x3::rule<VariableDeclaration_class, ast::VariableDeclaration>;
using Assignment_type = x3::rule<Assignment_class, ast::Assignment>;
using Variable_type = x3::rule<Variable_class, ast::Variable>;

auto const Statement = Statement_type("Statement");
auto const StatementList = StatementList_type("StatementList");
auto const VariableDeclaration = VariableDeclaration_type("VariableDeclaration");
auto const Assignment = Assignment_type("Assignment");
auto const Variable = Variable_type("Variable");

// Import the expression rule
namespace {
auto const& Expression = tcc::Expression();
}

auto const StatementList_def = +(VariableDeclaration | Assignment);
auto const VariableDeclaration_def = lexeme["auto" >> !(alnum | '_')] > Assignment;
auto const Assignment_def = Variable > '=' > Expression > ';';
auto const Variable_def = identifier;
auto const Statement_def = StatementList;

BOOST_SPIRIT_DEFINE(Statement, StatementList, VariableDeclaration, Assignment, Variable);

struct Statement_class : error_handler_base, x3::annotate_on_success {};
struct Assignment_class : x3::annotate_on_success {};
struct Variable_class : x3::annotate_on_success {};
}  // namespace parser
}  // namespace tcc

namespace tcc {
parser::Statement_type const& GetStatement() { return parser::Statement; }
}  // namespace tcc
