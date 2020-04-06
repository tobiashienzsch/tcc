/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "tcc/parser-qi/annotation.hpp"
#include "tcc/parser-qi/error_handler.hpp"
#include "tcc/parser-qi/function.hpp"

namespace client {
namespace parser {
template <typename Iterator>
function<Iterator>::function(ErrorHandler<Iterator>& errorHandler) : function::base_type(start), body(errorHandler) {
  qi::_1_type _1;
  // qi::_2_type _2;
  qi::_3_type _3;
  qi::_4_type _4;

  qi::_val_type _val;
  qi::raw_type raw;
  qi::lexeme_type lexeme;
  qi::alpha_type alpha;
  qi::alnum_type alnum;
  qi::string_type string;

  using boost::phoenix::function;
  using qi::fail;
  using qi::on_error;
  using qi::on_success;

  typedef function<client::ErrorHandler<Iterator>> ErrorHandlerFunction;
  typedef function<client::annotation<Iterator>> AnnotateFunction;

  name = !body.expr.keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

  Identifier = name;
  argument_list = -(Identifier % ',');

  start = lexeme[(string("void") | string("int")) >> !(alnum | '_')]  // make sure we have whole words
          > Identifier > '(' > argument_list > ')' > '{' > body > '}';

  // Debugging and error handling and reporting support.
  BOOST_SPIRIT_DEBUG_NODES((Identifier)(argument_list)(start));

  // Error handling: on error in start, call error handler.
  on_error<fail>(start, ErrorHandlerFunction(errorHandler)("Error! Expecting ", _4, _3));

  // Annotation: on success in start, call annotation.
  on_success(Identifier, AnnotateFunction(errorHandler.iters)(_val, _1));
}
}  // namespace parser
}  // namespace client
