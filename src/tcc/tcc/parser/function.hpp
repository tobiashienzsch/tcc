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
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Function(ErrorHandler<Iterator>& errorHandler);

    Statement<Iterator> body;
    qi::rule<Iterator, std::string(), Skipper<Iterator>> name;
    qi::rule<Iterator, ast::Identifier(), Skipper<Iterator>> identifier;
    qi::rule<Iterator, std::vector<ast::Identifier>(), Skipper<Iterator>> argumentList;
    qi::rule<Iterator, ast::Function(), Skipper<Iterator>> start;
};
}  // namespace parser
}  // namespace tcc

#endif
