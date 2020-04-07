#if !defined(TCC_PARSER_QI_STATEMENT_HPP)
#define TCC_PARSER_QI_STATEMENT_HPP

#include "tcc/parser/expression.hpp"

namespace tcc {
namespace parser {
//  The statement grammar
template <typename Iterator>
struct Statement : qi::grammar<Iterator, ast::StatementList(), Skipper<Iterator>> {
  Statement(ErrorHandler<Iterator>& errorHandler);

  Expression<Iterator> expr;
  qi::rule<Iterator, ast::StatementList(), Skipper<Iterator>> CompoundStatement;
  qi::rule<Iterator, ast::StatementList(), Skipper<Iterator>> StatementList;

  qi::rule<Iterator, ast::Statement(), Skipper<Iterator>> Statement_;
  qi::rule<Iterator, ast::VariableDeclaration(), Skipper<Iterator>> VariableDeclaration;
  qi::rule<Iterator, ast::Assignment(), Skipper<Iterator>> Assignment;
  qi::rule<Iterator, ast::IfStatement(), Skipper<Iterator>> IfStatement;
  qi::rule<Iterator, ast::WhileStatement(), Skipper<Iterator>> WhileStatement;
  qi::rule<Iterator, ast::ReturnStatement(), Skipper<Iterator>> ReturnStatement;
  qi::rule<Iterator, std::string(), Skipper<Iterator>> Identifier;
};
}  // namespace parser
}  // namespace tcc

#endif
