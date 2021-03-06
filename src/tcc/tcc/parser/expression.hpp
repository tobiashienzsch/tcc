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
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Expression(ErrorHandler<Iterator>& errorHandler);

    qi::rule<Iterator, ast::Expression(), Skipper<Iterator>> expr, equalityExpr, relationalExpr, logicalOrExpr,
        logicalAndExpr, additiveExpr, multiplicativeExpr;

    qi::rule<Iterator, ast::Operand(), Skipper<Iterator>> unaryExpr, primaryExpr;

    qi::rule<Iterator, ast::FunctionCall(), Skipper<Iterator>> functionCall;

    qi::rule<Iterator, std::vector<ast::Expression>(), Skipper<Iterator>> argumentList;

    qi::rule<Iterator, std::string(), Skipper<Iterator>> identifier;

    qi::symbols<char, ast::OpToken> logicalOrOp, logicalAndOp, equalityOp, relationalOp, additiveOp, multiplicativeOp,
        unaryOp;

    qi::symbols<char> keywords;
};
}  // namespace parser
}  // namespace tcc

#endif
