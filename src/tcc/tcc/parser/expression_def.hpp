#include <boost/spirit/include/phoenix_function.hpp>

#include "tcc/parser/annotation.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/expression.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
namespace parser
{
template<typename Iterator>
Expression<Iterator>::Expression(ErrorHandler<Iterator>& errorHandler) : Expression::base_type(expr)
{
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    tcc::ignoreUnused(_2);

    qi::char_type char_;
    qi::uint_type uint;
    qi::_val_type val;
    qi::raw_type raw;
    qi::lexeme_type lexeme;
    qi::alpha_type alpha;
    qi::alnum_type alnum;
    qi::bool_type bool_;

    using boost::phoenix::function;
    using qi::fail;
    using qi::on_error;
    using qi::on_success;

    using ErrorHandlerFunction = function<tcc::ErrorHandler<Iterator>>;
    using AnnotationFunction   = function<tcc::Annotation<Iterator>>;

    // clang-format off
    logicalOrOp.add
        ("||", ast::OpToken::Or)
        ;

    logicalAndOp.add
        ("&&", ast::OpToken::And)
        ;

    equalityOp.add
        ("==", ast::OpToken::Equal)
        ("!=", ast::OpToken::NotEqual)
        ;

    relationalOp.add
        ("<", ast::OpToken::Less)
        ("<=", ast::OpToken::LessEqual)
        (">", ast::OpToken::Greater)
        (">=", ast::OpToken::GreaterEqual)
        ;

    additiveOp.add
        ("+", ast::OpToken::Plus)
        ("-", ast::OpToken::Minus)
        ;

    multiplicativeOp.add
        ("*", ast::OpToken::Times)
        ("/", ast::OpToken::Divide)
        ;

    unaryOp.add
        ("+", ast::OpToken::Positive)
        ("-", ast::OpToken::Negative)
        ("!", ast::OpToken::Not)
        ;

    keywords.add
        ("true")
        ("false")
        ("if")
        ("else")
        ("while")
        ("int")
        ("void")
        ("return")
        ;

    // Main expression grammar
    expr =
        logicalOrExpr.alias()
        ;

    logicalOrExpr =
            logicalAndExpr
        >> *(logicalOrOp > logicalAndExpr)
        ;

    logicalAndExpr =
            equalityExpr
        >> *(logicalAndOp > equalityExpr)
        ;

    equalityExpr =
            relationalExpr
        >> *(equalityOp > relationalExpr)
        ;

    relationalExpr =
            additiveExpr
        >> *(relationalOp > additiveExpr)
        ;

    additiveExpr =
            multiplicativeExpr
        >> *(additiveOp > multiplicativeExpr)
        ;

    multiplicativeExpr =
            unaryExpr
        >> *(multiplicativeOp > unaryExpr)
        ;

    unaryExpr =
            primaryExpr
        |   (unaryOp > unaryExpr)
        ;

    primaryExpr =
            uint
        |   functionCall
        |   identifier
        |   bool_
        |   '(' > expr > ')'
        ;

    functionCall =
            (identifier >> '(')
        >   argumentList
        >   ')'
        ;

    argumentList = -(expr % ',');

    identifier =
            !lexeme[keywords >> !(alnum | '_')]
        >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
        ;

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODES(
        (expr)
        (logicalOrExpr)
        (logicalAndExpr)
        (equalityExpr)
        (relationalExpr)
        (additiveExpr)
        (multiplicativeExpr)
        (unaryExpr)
        (primaryExpr)
        (functionCall)
        (argumentList)
        (identifier)
    );

    // Error handling: on error in expr, call errorHandler.
    on_error<fail>(expr,
        ErrorHandlerFunction(errorHandler)(
            "Error! Expecting ", _4, _3));

    // Annotation: on success in primary_expr, call Annotation.
    on_success(primaryExpr,
        AnnotationFunction(errorHandler.getIterators())(val, _1));

    // clang-format off

    }
}}


