#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "tcc/parser/ast.hpp"
#include "tcc/parser/ast_adapted.hpp"
#include "tcc/parser/common.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/expression.hpp"

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

void add_keywords();

////////////////////////////////////////////////////////////////////////////
// Main expression grammar
////////////////////////////////////////////////////////////////////////////

struct EqualityExpr_class;
struct RelationalExpr_class;
struct LogicalExpr_class;
struct AdditiveExpr_class;
struct MultiplicativeExpr_class;
struct UnaryExpr_class;
struct PrimaryExpr_class;

using EqualityExpr_type = x3::rule<EqualityExpr_class, ast::Expression>;
using RelationalExpr_type = x3::rule<RelationalExpr_class, ast::Expression>;
using LogicalExpr_type = x3::rule<LogicalExpr_class, ast::Expression>;
using AdditiveExpr_type = x3::rule<AdditiveExpr_class, ast::Expression>;
using MultiplicativeExpr_type = x3::rule<MultiplicativeExpr_class, ast::Expression>;
using UnaryExpr_type = x3::rule<UnaryExpr_class, ast::Operand>;
using PrimaryExpr_type = x3::rule<PrimaryExpr_class, ast::Operand>;

Expression_type const Expression = "Expression";
EqualityExpr_type const EqualityExpr = "EqualityExpr";
RelationalExpr_type const RelationalExpr = "RelationalExpr";
LogicalExpr_type const LogicalExpr = "LogicalExpr";
AdditiveExpr_type const AdditiveExpr = "AdditiveExpr";
MultiplicativeExpr_type const MultiplicativeExpr = "MultiplicativeExpr";
UnaryExpr_type const UnaryExpr = "UnaryExpr";
PrimaryExpr_type const PrimaryExpr = "PrimaryExpr";

auto const LogicalExpr_def = EqualityExpr >> *(LogicalOperators > EqualityExpr);
auto const EqualityExpr_def = RelationalExpr >> *(EqualityOperators > RelationalExpr);
auto const RelationalExpr_def = AdditiveExpr >> *(RelationalOperators > AdditiveExpr);
auto const AdditiveExpr_def = MultiplicativeExpr >> *(AdditiveOperators > MultiplicativeExpr);
auto const MultiplicativeExpr_def = UnaryExpr >> *(MultiplicativeOperators > UnaryExpr);
auto const UnaryExpr_def = PrimaryExpr | (UnaryOperators > PrimaryExpr);
auto const PrimaryExpr_def = uint_ | bool_ | (!keywords >> identifier) | ('(' > Expression > ')');
auto const Expression_def = LogicalExpr;

BOOST_SPIRIT_DEFINE(     //
    Expression,          //
    LogicalExpr,         //
    EqualityExpr,        //
    RelationalExpr,      //
    AdditiveExpr,        //
    MultiplicativeExpr,  //
    UnaryExpr,           //
    PrimaryExpr          //
)

struct UnaryExpr_class : x3::annotate_on_success {};
struct PrimaryExpr_class : x3::annotate_on_success {};

}  // namespace parser
}  // namespace tcc
