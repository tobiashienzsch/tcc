#if !defined(TCC_PARSER_QI_EXPRESSION_HPP)
#define TCC_PARSER_QI_EXPRESSION_HPP

// Spirit v2.5 allows you to suppress automatic generation
// of predefined terminals to speed up complation. With
// BOOST_SPIRIT_NO_PREDEFINED_TERMINALS defined, you are
// responsible in creating instances of the terminals that
// you need (e.g. see qi::uint_type uint_ below).
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

// Uncomment this if you want to enable debugging
// #define BOOST_SPIRIT_QI_DEBUG

#include <boost/spirit/include/qi.hpp>
#include <vector>

#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/skipper.hpp"

namespace tcc
{
namespace parser
{
namespace qi    = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

//  The expression grammar
template<typename Iterator>
struct Expression : qi::grammar<Iterator, ast::Expression(), Skipper<Iterator>>
{
    Expression(ErrorHandler<Iterator>& errorHandler);

    qi::rule<Iterator, ast::Expression(), Skipper<Iterator>> expr, equality_expr, relational_expr,
        logical_or_expr, logical_and_expr, additive_expr, multiplicative_expr;

    qi::rule<Iterator, ast::Operand(), Skipper<Iterator>> UnaryExpr, primary_expr;

    qi::rule<Iterator, ast::FunctionCall(), Skipper<Iterator>> FunctionCall;

    qi::rule<Iterator, std::list<ast::Expression>(), Skipper<Iterator>> argument_list;

    qi::rule<Iterator, std::string(), Skipper<Iterator>> Identifier;

    qi::symbols<char, ast::OpToken> logical_or_op, logical_and_op, equality_op, relational_op,
        additive_op, multiplicative_op, UnaryOp;

    qi::symbols<char> keywords;
};
}  // namespace parser
}  // namespace tcc

#endif
