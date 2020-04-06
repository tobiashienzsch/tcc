#if !defined(TCC_PARSER_QI_STATEMENT_HPP)
#define TCC_PARSER_QI_STATEMENT_HPP

#include "tcc/parser-qi/expression.hpp"

namespace client {
namespace parser {
///////////////////////////////////////////////////////////////////////////////
//  The statement grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct Statement : qi::grammar<Iterator, ast::StatementList(), skipper<Iterator>> {
  Statement(ErrorHandler<Iterator>& errorHandler);

  Expression<Iterator> expr;
  qi::rule<Iterator, ast::StatementList(), skipper<Iterator>> StatementList, compound_statement;

  qi::rule<Iterator, ast::Statement(), skipper<Iterator>> Statement_;
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
