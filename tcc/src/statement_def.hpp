#pragma once

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"
#include "statement.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace tcc
{
namespace parser
{
using x3::lexeme;
using x3::raw;
using namespace x3::ascii;

struct statement_list_class;
struct variable_declaration_class;
struct assignment_class;
struct variable_class;

using statement_list_type       = x3::rule<statement_list_class, ast::statement_list>;
using variable_declaration_type = x3::rule<variable_declaration_class, ast::variable_declaration>;
using assignment_type           = x3::rule<assignment_class, ast::assignment>;
using variable_type             = x3::rule<variable_class, ast::variable>;

auto const statement            = statement_type("statement");
auto const statement_list       = statement_list_type("statement_list");
auto const variable_declaration = variable_declaration_type("variable_declaration");
auto const assignment           = assignment_type("assignment");
auto const variable             = variable_type("variable");

// Import the expression rule
namespace
{
auto const& expression = tcc::expression();
}

auto const statement_list_def       = +(variable_declaration | assignment);
auto const variable_declaration_def = lexeme["auto" >> !(alnum | '_')] > assignment;
auto const assignment_def           = variable > '=' > expression > ';';
auto const variable_def             = identifier;
auto const statement_def            = statement_list;

BOOST_SPIRIT_DEFINE(statement, statement_list, variable_declaration, assignment, variable);

struct statement_class
    : error_handler_base
    , x3::annotate_on_success
{
};
struct assignment_class : x3::annotate_on_success
{
};
struct variable_class : x3::annotate_on_success
{
};
}  // namespace parser
}  // namespace tcc

namespace tcc
{
parser::statement_type const& statement() { return parser::statement; }
}  // namespace tcc
