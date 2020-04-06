#if !defined(BOOST_SPIRIT_MINIC_FUNCTION_HPP)
#define BOOST_SPIRIT_MINIC_FUNCTION_HPP

#include "tcc/parser-qi/statement.hpp"

namespace client {
namespace parser {

template <typename Iterator>
struct function : qi::grammar<Iterator, ast::function(), skipper<Iterator>> {
  function(ErrorHandler<Iterator>& errorHandler);

  statement<Iterator> body;
  qi::rule<Iterator, std::string(), skipper<Iterator>> name;
  qi::rule<Iterator, ast::Identifier(), skipper<Iterator>> Identifier;
  qi::rule<Iterator, std::list<ast::Identifier>(), skipper<Iterator>> argument_list;
  qi::rule<Iterator, ast::function(), skipper<Iterator>> start;
};
}  // namespace parser
}  // namespace client

#endif
