#if !defined(TCC_PARSER_QI_STATEMENT_HPP)
#define TCC_PARSER_QI_STATEMENT_HPP

#include "tcc/parser/expression.hpp"

namespace tcc
{
namespace parser
{
//  The statement grammar
template<typename Iterator>
struct Statement : qi::grammar<Iterator, ast::StatementList(), Skipper<Iterator>>
{
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Statement(ErrorHandler<Iterator>& errorHandler);

    Expression<Iterator> expr;
    qi::rule<Iterator, ast::StatementList(), Skipper<Iterator>> compoundStatement;
    qi::rule<Iterator, ast::StatementList(), Skipper<Iterator>> statementList;
    qi::rule<Iterator, ast::Statement(), Skipper<Iterator>> statement;
    qi::rule<Iterator, ast::VariableDeclaration(), Skipper<Iterator>> variableDeclaration;
    qi::rule<Iterator, ast::Assignment(), Skipper<Iterator>> assignment;
    qi::rule<Iterator, ast::IfStatement(), Skipper<Iterator>> ifStatement;
    qi::rule<Iterator, ast::WhileStatement(), Skipper<Iterator>> whileStatement;
    qi::rule<Iterator, ast::ReturnStatement(), Skipper<Iterator>> returnStatement;
    qi::rule<Iterator, std::string(), Skipper<Iterator>> identifier;
};
}  // namespace parser
}  // namespace tcc

#endif
