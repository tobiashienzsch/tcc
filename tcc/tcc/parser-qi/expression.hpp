/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINIC_EXPRESSION_HPP)
#define BOOST_SPIRIT_MINIC_EXPRESSION_HPP

///////////////////////////////////////////////////////////////////////////////
// Spirit v2.5 allows you to suppress automatic generation
// of predefined terminals to speed up complation. With
// BOOST_SPIRIT_NO_PREDEFINED_TERMINALS defined, you are
// responsible in creating instances of the terminals that
// you need (e.g. see qi::uint_type uint_ below).
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Uncomment this if you want to enable debugging
// #define BOOST_SPIRIT_QI_DEBUG
///////////////////////////////////////////////////////////////////////////////

#include <boost/spirit/include/qi.hpp>
#include <vector>

#include "tcc/parser-qi/ast.hpp"
#include "tcc/parser-qi/error_handler.hpp"
#include "tcc/parser-qi/skipper.hpp"

namespace client {
namespace parser {
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

///////////////////////////////////////////////////////////////////////////////
//  The expression grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct expression : qi::grammar<Iterator, ast::expression(), skipper<Iterator>> {
  expression(ErrorHandler<Iterator>& errorHandler);

  qi::rule<Iterator, ast::expression(), skipper<Iterator>> expr, equality_expr, relational_expr, logical_or_expr,
      logical_and_expr, additive_expr, multiplicative_expr;

  qi::rule<Iterator, ast::operand(), skipper<Iterator>> UnaryExpr, primary_expr;

  qi::rule<Iterator, ast::FunctionCall(), skipper<Iterator>> FunctionCall;

  qi::rule<Iterator, std::list<ast::expression>(), skipper<Iterator>> argument_list;

  qi::rule<Iterator, std::string(), skipper<Iterator>> Identifier;

  qi::symbols<char, ast::optoken> logical_or_op, logical_and_op, equality_op, relational_op, additive_op,
      multiplicative_op, UnaryOp;

  qi::symbols<char> keywords;
};
}  // namespace parser
}  // namespace client

#endif
