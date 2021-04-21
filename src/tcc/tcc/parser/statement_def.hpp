#include "tcc/parser/annotation.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/statement.hpp"
#include "tcsl/tcsl.hpp"

namespace tcc
{
namespace parser
{
template<typename Iterator>
Statement<Iterator>::Statement(ErrorHandler<Iterator>& errorHandler)
    : Statement::base_type(statementList), expr(errorHandler)
{
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    tcc::ignoreUnused(_2);

    qi::_val_type val;
    qi::raw_type raw;
    qi::lexeme_type lexeme;
    qi::alpha_type alpha;
    qi::alnum_type alnum;
    qi::lit_type lit;

    using boost::phoenix::function;
    using qi::fail;
    using qi::on_error;
    using qi::on_success;

    using ErrorHandlerFunction = function<tcc::ErrorHandler<Iterator>>;
    using AnnotateFunction     = function<tcc::Annotation<Iterator>>;

    // clang-format off
    statementList =
        +statement
        ;

    statement =
            variableDeclaration
        |   assignment
        |   compoundStatement
        |   ifStatement
        |   whileStatement
        |   returnStatement
        ;

    identifier =
            !expr.keywords
        >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
        ;

    variableDeclaration =
            lexeme["int" >> !(alnum | '_')] // make sure we have whole words
        >   identifier
        >   -('=' > expr)
        >   ';'
        ;

    assignment =
            identifier
        >   '='
        >   expr
        >   ';'
        ;

    ifStatement =
            lit("if")
        >   '('
        >   expr
        >   ')'
        >   statement
        >
            -(
                lexeme["else" >> !(alnum | '_')] // make sure we have whole words
            >   statement
            )
        ;

    whileStatement =
            lit("while")
        >   '('
        >   expr
        >   ')'
        >   statement
        ;

    compoundStatement =
        '{' >> -statementList >> '}'
        ;

    returnStatement =
            lexeme["return" >> !(alnum | '_')] // make sure we have whole words
        >  -expr
        >   ';'
        ;
    // clang-format on

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODES((statementList)(identifier)(variableDeclaration)(assignment));

    // Error handling: on error in StatementList, call error handler.
    on_error<fail>(statementList, ErrorHandlerFunction(errorHandler)("Error! Expecting ", _4, _3));

    // Annotation: on success in VariableDeclaration,
    // Assignment and ReturnStatement, call annotation.
    on_success(variableDeclaration, AnnotateFunction(errorHandler.getIterators())(val, _1));
    on_success(assignment, AnnotateFunction(errorHandler.getIterators())(val, _1));
    on_success(returnStatement, AnnotateFunction(errorHandler.getIterators())(val, _1));
}
}  // namespace parser
}  // namespace tcc
