/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINIC_STATEMENT_HPP)
#define BOOST_SPIRIT_MINIC_STATEMENT_HPP

#include "tcc/parser-qi/expression.hpp"

namespace client {
namespace parser {
///////////////////////////////////////////////////////////////////////////////
//  The statement grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct statement : qi::grammar<Iterator, ast::StatementList(), skipper<Iterator>> {
  statement(error_handler<Iterator>& error_handler);

  expression<Iterator> expr;
  qi::rule<Iterator, ast::StatementList(), skipper<Iterator>> StatementList, compound_statement;

  qi::rule<Iterator, ast::statement(), skipper<Iterator>> statement_;
  qi::rule<Iterator, ast::VariableDeclaration(), skipper<Iterator>> VariableDeclaration;
  qi::rule<Iterator, ast::Assignment(), skipper<Iterator>> Assignment;
  qi::rule<Iterator, ast::IfStatement(), skipper<Iterator>> IfStatement;
  qi::rule<Iterator, ast::WhileStatement(), skipper<Iterator>> WhileStatement;
  qi::rule<Iterator, ast::ReturnStatement(), skipper<Iterator>> ReturnStatement;
  qi::rule<Iterator, std::string(), skipper<Iterator>> Identifier;
};
}  // namespace parser
}  // namespace client

#endif
