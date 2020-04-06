#include "tcc/parser-qi/annotation.hpp"
#include "tcc/parser-qi/error_handler.hpp"
#include "tcc/parser-qi/statement.hpp"

namespace client {
namespace parser {
template <typename Iterator>
Statement<Iterator>::Statement(ErrorHandler<Iterator>& errorHandler)
    : Statement::base_type(StatementList), expr(errorHandler) {
  qi::_1_type _1;
  //   qi::_2_type _2;
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

  using ErrorHandlerFunction = function<client::ErrorHandler<Iterator>>;
  using AnnotateFunction = function<client::annotation<Iterator>>;

  // clang-format off
    StatementList =
        +Statement_
        ;

    Statement_ =
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
        >   Statement_
        >
            -(
                lexeme["else" >> !(alnum | '_')] // make sure we have whole words
            >   Statement_
            )
        ;

    WhileStatement =
            lit("while")
        >   '('
        >   expr
        >   ')'
        >   Statement_
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

  // Error handling: on error in StatementList, call error handler.
  on_error<fail>(StatementList, ErrorHandlerFunction(errorHandler)("Error! Expecting ", _4, _3));

  // Annotation: on success in VariableDeclaration,
  // Assignment and ReturnStatement, call annotation.
  on_success(VariableDeclaration, AnnotateFunction(errorHandler.iters)(_val, _1));
  on_success(Assignment, AnnotateFunction(errorHandler.iters)(_val, _1));
  on_success(ReturnStatement, AnnotateFunction(errorHandler.iters)(_val, _1));
}
}  // namespace parser
}  // namespace client