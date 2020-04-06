/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_MINIC_FUNCTION_HPP)
#define BOOST_SPIRIT_MINIC_FUNCTION_HPP

#include "tcc/parser-qi/statement.hpp"

namespace client {
namespace parser {
///////////////////////////////////////////////////////////////////////////////
//  The function grammar
///////////////////////////////////////////////////////////////////////////////
template <typename Iterator>
struct function : qi::grammar<Iterator, ast::function(), skipper<Iterator>> {
  function(error_handler<Iterator>& error_handler);

  statement<Iterator> body;
  qi::rule<Iterator, std::string(), skipper<Iterator>> name;
  qi::rule<Iterator, ast::Identifier(), skipper<Iterator>> Identifier;
  qi::rule<Iterator, std::list<ast::Identifier>(), skipper<Iterator>> argument_list;
  qi::rule<Iterator, ast::function(), skipper<Iterator>> start;
};
}  // namespace parser
}  // namespace client

#endif
