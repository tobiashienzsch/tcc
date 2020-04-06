#if !defined(TCC_PARSER_QI_FUNCTION_HPP)
#define TCC_PARSER_QI_FUNCTION_HPP

#include "tcc/parser/statement.hpp"

namespace tcc {
namespace parser {

template <typename Iterator>
struct function : qi::grammar<Iterator, ast::function(), Skipper<Iterator>> {
  function(ErrorHandler<Iterator>& errorHandler);

  Statement<Iterator> body;
  qi::rule<Iterator, std::string(), Skipper<Iterator>> name;
  qi::rule<Iterator, ast::Identifier(), Skipper<Iterator>> Identifier;
  qi::rule<Iterator, std::list<ast::Identifier>(), Skipper<Iterator>> argument_list;
  qi::rule<Iterator, ast::function(), Skipper<Iterator>> start;
};
}  // namespace parser
}  // namespace tcc

#endif
