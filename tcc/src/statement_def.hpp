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
struct variable_declaration_class;
struct assignment_class;
struct variable_class;

using StatementList_type = x3::rule<StatementList_class, ast::StatementList>;
using variable_declaration_type = x3::rule<variable_declaration_class, ast::variable_declaration>;
using assignment_type = x3::rule<assignment_class, ast::assignment>;
using variable_type = x3::rule<variable_class, ast::variable>;

auto const Statement = Statement_type("Statement");
auto const StatementList = StatementList_type("StatementList");
auto const variable_declaration = variable_declaration_type("variable_declaration");
auto const assignment = assignment_type("assignment");
auto const variable = variable_type("variable");

// Import the expression rule
namespace {
auto const& Expression = tcc::Expression();
}

auto const StatementList_def = +(variable_declaration | assignment);
auto const variable_declaration_def = lexeme["auto" >> !(alnum | '_')] > assignment;
auto const assignment_def = variable > '=' > Expression > ';';
auto const variable_def = identifier;
auto const Statement_def = StatementList;

BOOST_SPIRIT_DEFINE(Statement, StatementList, variable_declaration, assignment, variable);

struct Statement_class : error_handler_base, x3::annotate_on_success {};
struct assignment_class : x3::annotate_on_success {};
struct variable_class : x3::annotate_on_success {};
}  // namespace parser
}  // namespace tcc

namespace tcc {
parser::Statement_type const& GetStatement() { return parser::Statement; }
}  // namespace tcc
