#include <boost/spirit/include/phoenix_function.hpp>

#include "tcc/parser-qi/annotation.hpp"
#include "tcc/parser-qi/error_handler.hpp"
#include "tcc/parser-qi/expression.hpp"

namespace client {
namespace parser {
template <typename Iterator>
Expression<Iterator>::Expression(ErrorHandler<Iterator>& errorHandler) : Expression::base_type(expr) {
  qi::_1_type _1;
  //   qi::_2_type _2;
  qi::_3_type _3;
  qi::_4_type _4;

  qi::char_type char_;
  qi::uint_type uint_;
  qi::_val_type _val;
  qi::raw_type raw;
  qi::lexeme_type lexeme;
  qi::alpha_type alpha;
  qi::alnum_type alnum;
  qi::bool_type bool_;

  using boost::phoenix::function;
  using qi::fail;
  using qi::on_error;
  using qi::on_success;

  typedef function<client::ErrorHandler<Iterator>> ErrorHandlerFunction;
  typedef function<client::annotation<Iterator>> AnnotationFunction;

  ///////////////////////////////////////////////////////////////////////
  // Tokens
  // clang-format off
        logical_or_op.add
            ("||", ast::op_or)
            ;

        logical_and_op.add
            ("&&", ast::op_and)
            ;

        equality_op.add
            ("==", ast::op_equal)
            ("!=", ast::op_not_equal)
            ;

        relational_op.add
            ("<", ast::op_less)
            ("<=", ast::op_less_equal)
            (">", ast::op_greater)
            (">=", ast::op_greater_equal)
            ;

        additive_op.add
            ("+", ast::op_plus)
            ("-", ast::op_minus)
            ;

        multiplicative_op.add
            ("*", ast::op_times)
            ("/", ast::op_divide)
            ;

        UnaryOp.add
            ("+", ast::op_positive)
            ("-", ast::op_negative)
            ("!", ast::op_not)
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

        ///////////////////////////////////////////////////////////////////////
        // Main expression grammar
        expr =
            logical_or_expr.alias()
            ;

        logical_or_expr =
                logical_and_expr
            >> *(logical_or_op > logical_and_expr)
            ;

        logical_and_expr =
                equality_expr
            >> *(logical_and_op > equality_expr)
            ;

        equality_expr =
                relational_expr
            >> *(equality_op > relational_expr)
            ;

        relational_expr =
                additive_expr
            >> *(relational_op > additive_expr)
            ;

        additive_expr =
                multiplicative_expr
            >> *(additive_op > multiplicative_expr)
            ;

        multiplicative_expr =
                UnaryExpr
            >> *(multiplicative_op > UnaryExpr)
            ;

        UnaryExpr =
                primary_expr
            |   (UnaryOp > UnaryExpr)
            ;

        primary_expr =
                uint_
            |   FunctionCall
            |   Identifier
            |   bool_
            |   '(' > expr > ')'
            ;

        FunctionCall =
                (Identifier >> '(')
            >   argument_list
            >   ')'
            ;

        argument_list = -(expr % ',');

        Identifier =
                !lexeme[keywords >> !(alnum | '_')]
            >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
            ;

        ///////////////////////////////////////////////////////////////////////
        // Debugging and error handling and reporting support.
        BOOST_SPIRIT_DEBUG_NODES(
            (expr)
            (logical_or_expr)
            (logical_and_expr)
            (equality_expr)
            (relational_expr)
            (additive_expr)
            (multiplicative_expr)
            (UnaryExpr)
            (primary_expr)
            (FunctionCall)
            (argument_list)
            (Identifier)
        );

        ///////////////////////////////////////////////////////////////////////
        // Error handling: on error in expr, call errorHandler.
        on_error<fail>(expr,
            ErrorHandlerFunction(errorHandler)(
                "Error! Expecting ", _4, _3));

        ///////////////////////////////////////////////////////////////////////
        // Annotation: on success in primary_expr, call annotation.
        on_success(primary_expr,
            AnnotationFunction(errorHandler.iters)(_val, _1));

        // clang-format off

    }
}}


