/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "annotation.hpp"
#include "error_handler.hpp"
#include "statement.hpp"

namespace client {
namespace parser {
template <typename Iterator>
statement<Iterator>::statement(error_handler<Iterator>& error_handler)
    : statement::base_type(StatementList), expr(error_handler) {
  qi::_1_type _1;
  qi::_2_type _2;
  qi::_3_type _3;
  qi::_4_type _4;

  qi::_val_type _val;
  qi::raw_type raw;
  qi::lexeme_type lexeme;
  qi::alpha_type alpha;
  qi::alnum_type alnum;
  qi::lit_type lit;

  using boost::phoenix::function;
  using qi::fail;
  using qi::on_error;
  using qi::on_success;

  typedef function<client::error_handler<Iterator>> error_handler_function;
  typedef function<client::annotation<Iterator>> annotation_function;

  // clang-format off
    StatementList =
        +statement_
        ;

    statement_ =
            VariableDeclaration
        |   Assignment
        |   compound_statement
        |   IfStatement
        |   WhileStatement
        |   ReturnStatement
        ;

    Identifier =
            !expr.keywords
        >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
        ;

    VariableDeclaration =
            lexeme["int" >> !(alnum | '_')] // make sure we have whole words
        >   Identifier
        >   -('=' > expr)
        >   ';'
        ;

    Assignment =
            Identifier
        >   '='
        >   expr
        >   ';'
        ;

    IfStatement =
            lit("if")
        >   '('
        >   expr
        >   ')'
        >   statement_
        >
            -(
                lexeme["else" >> !(alnum | '_')] // make sure we have whole words
            >   statement_
            )
        ;

    WhileStatement =
            lit("while")
        >   '('
        >   expr
        >   ')'
        >   statement_
        ;

    compound_statement =
        '{' >> -StatementList >> '}'
        ;

    ReturnStatement =
            lexeme["return" >> !(alnum | '_')] // make sure we have whole words
        >  -expr
        >   ';'
        ;
  // clang-format on

  // Debugging and error handling and reporting support.
  BOOST_SPIRIT_DEBUG_NODES((StatementList)(Identifier)(VariableDeclaration)(Assignment));

  // Error handling: on error in StatementList, call error_handler.
  on_error<fail>(StatementList, error_handler_function(error_handler)("Error! Expecting ", _4, _3));

  // Annotation: on success in VariableDeclaration,
  // Assignment and ReturnStatement, call annotation.
  on_success(VariableDeclaration, annotation_function(error_handler.iters)(_val, _1));
  on_success(Assignment, annotation_function(error_handler.iters)(_val, _1));
  on_success(ReturnStatement, annotation_function(error_handler.iters)(_val, _1));
}
}  // namespace parser
}  // namespace client
