#include "tcc/parser/annotation.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/function.hpp"
#include "tsl/tsl.hpp"

namespace tcc
{
namespace parser
{
template<typename Iterator>
Function<Iterator>::Function(ErrorHandler<Iterator>& errorHandler)
    : Function::base_type(start)
    , body(errorHandler)
{
    qi::_1_type _1;
    qi::_2_type _2;
    qi::_3_type _3;
    qi::_4_type _4;

    tcc::IgnoreUnused(_2);

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

    using ErrorHandlerFunction = function<tcc::ErrorHandler<Iterator>>;
    using AnnotateFunction     = function<tcc::Annotation<Iterator>>;

    name = !body.expr.keywords >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

    Identifier    = name;
    argument_list = -(Identifier % ',');

    start = lexeme[(string("void") | string("int"))
                   >> !(alnum | '_')]  // make sure we have whole words
            > Identifier > '(' > argument_list > ')' > '{' > body > '}';

    // Debugging and error handling and reporting support.
    BOOST_SPIRIT_DEBUG_NODES((Identifier)(argument_list)(start));

    // Error handling: on error in start, call error handler.
    on_error<fail>(
        start, ErrorHandlerFunction(errorHandler)("Error! Expecting ", _4, _3));

    // Annotation: on success in start, call annotation.
    on_success(Identifier,
               AnnotateFunction(errorHandler.GetIterators())(_val, _1));
}
}  // namespace parser
}  // namespace tcc
