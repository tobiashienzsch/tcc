#if !defined(TCC_PARSER_QI_FUNCTION_HPP)
#define TCC_PARSER_QI_FUNCTION_HPP

#include "tcc/parser/statement.hpp"

namespace tcc
{
namespace parser
{

template<typename Iterator>
struct Function : qi::grammar<Iterator, ast::Function(), Skipper<Iterator>>
{
    Function(ErrorHandler<Iterator>& errorHandler);

    Statement<Iterator> Body;
    qi::rule<Iterator, std::string(), Skipper<Iterator>> name;
    qi::rule<Iterator, ast::Identifier(), Skipper<Iterator>> Identifier;
    qi::rule<Iterator, std::list<ast::Identifier>(), Skipper<Iterator>> argument_list;
    qi::rule<Iterator, ast::Function(), Skipper<Iterator>> start;
};
}  // namespace parser
}  // namespace tcc

#endif
