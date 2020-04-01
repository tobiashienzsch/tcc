#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"

namespace tcc {
namespace parser {
using x3::bool_;
using x3::char_;
using x3::lexeme;
using x3::raw;
using x3::uint_;
using namespace x3::ascii;

////////////////////////////////////////////////////////////////////////////
// Tokens
////////////////////////////////////////////////////////////////////////////

x3::symbols<ast::OpToken> EqualityOperators;
x3::symbols<ast::OpToken> RelationalOperators;
x3::symbols<ast::OpToken> LogicalOperators;
x3::symbols<ast::OpToken> AdditiveOperators;
x3::symbols<ast::OpToken> MultiplicativeOperators;
x3::symbols<ast::OpToken> UnaryOperators;
x3::symbols<> keywords;

void add_keywords() {
  static bool once = false;
  if (once) return;
  once = true;

  LogicalOperators.add("&&", ast::OpToken::And);
  LogicalOperators.add("||", ast::OpToken::Or);

  EqualityOperators.add("==", ast::OpToken::Equal);
  EqualityOperators.add("!=", ast::OpToken::NotEqual);

  RelationalOperators.add("<", ast::OpToken::Less);
  RelationalOperators.add("<=", ast::OpToken::LessEqual);
  RelationalOperators.add(">", ast::OpToken::Greater);
  RelationalOperators.add(">=", ast::OpToken::GreaterEqual);

  AdditiveOperators.add("+", ast::OpToken::Plus);
  AdditiveOperators.add("-", ast::OpToken::Minus);

  MultiplicativeOperators.add("*", ast::OpToken::Times);
  MultiplicativeOperators.add("/", ast::OpToken::Divide);

  UnaryOperators.add("+", ast::OpToken::Positive);
  UnaryOperators.add("-", ast::OpToken::Negative);
  UnaryOperators.add("!", ast::OpToken::Not);

  keywords.add("auto");
  keywords.add("true");
  keywords.add("false");
  keywords.add("if");
  keywords.add("else");
  keywords.add("while");
}

////////////////////////////////////////////////////////////////////////////
// Main expression grammar
////////////////////////////////////////////////////////////////////////////

struct equality_expr_class;
struct relational_expr_class;
struct logical_expr_class;
struct additive_expr_class;
struct multiplicative_expr_class;
struct UnaryExpr_class;
struct primary_expr_class;

using equality_expr_type = x3::rule<equality_expr_class, ast::Expression>;
using relational_expr_type = x3::rule<relational_expr_class, ast::Expression>;
using logical_expr_type = x3::rule<logical_expr_class, ast::Expression>;
using additive_expr_type = x3::rule<additive_expr_class, ast::Expression>;
using multiplicative_expr_type = x3::rule<multiplicative_expr_class, ast::Expression>;
using UnaryExpr_type = x3::rule<UnaryExpr_class, ast::Operand>;
using primary_expr_type = x3::rule<primary_expr_class, ast::Operand>;

Expression_type const Expression = "Expression";
equality_expr_type const equality_expr = "equality_expr";
relational_expr_type const relational_expr = "relational_expr";
logical_expr_type const logical_expr = "logical_expr";
additive_expr_type const additive_expr = "additive_expr";
multiplicative_expr_type const multiplicative_expr = "multiplicative_expr";
UnaryExpr_type const UnaryExpr = "UnaryExpr";
primary_expr_type const primary_expr = "primary_expr";

auto const logical_expr_def = equality_expr >> *(LogicalOperators > equality_expr);
auto const equality_expr_def = relational_expr >> *(EqualityOperators > relational_expr);
auto const relational_expr_def = additive_expr >> *(RelationalOperators > additive_expr);
auto const additive_expr_def = multiplicative_expr >> *(AdditiveOperators > multiplicative_expr);
auto const multiplicative_expr_def = UnaryExpr >> *(MultiplicativeOperators > UnaryExpr);
auto const UnaryExpr_def = primary_expr | (UnaryOperators > primary_expr);
auto const primary_expr_def = uint_ | bool_ | (!keywords >> identifier) | '(' > Expression > ')';
auto const Expression_def = logical_expr;

BOOST_SPIRIT_DEFINE(      //
    Expression,           //
    logical_expr,         //
    equality_expr,        //
    relational_expr,      //
    additive_expr,        //
    multiplicative_expr,  //
    UnaryExpr,            //
    primary_expr          //
);

struct UnaryExpr_class : x3::annotate_on_success {};
struct primary_expr_class : x3::annotate_on_success {};

}  // namespace parser
}  // namespace tcc

namespace tcc {
parser::Expression_type const& Expression() {
  parser::add_keywords();
  return parser::Expression;
}
}  // namespace tcc
