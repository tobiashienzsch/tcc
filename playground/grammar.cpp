#include "grammar.hpp"

namespace client
{
///////////////////////////////////////////////////////////////////////////////
//  The calculator grammar
///////////////////////////////////////////////////////////////////////////////
namespace calculator_grammar
{
using x3::char_;
using x3::uint_;

static auto expression = []() -> x3::rule<class expression, ast::program> const&
{
    try
    {
        static x3::rule<class expression, ast::program> expression("expression");
        return expression;
    }
    catch (...)
    {
    }
}
();

x3::rule<class term, ast::program> const term("term");
x3::rule<class factor, ast::operand> const factor("factor");

auto const expression_def = term >> *((char_('+') >> term) | (char_('-') >> term));

auto const term_def = factor >> *((char_('*') >> factor) | (char_('/') >> factor));

auto const factor_def = uint_ | '(' >> expression >> ')' | (char_('-') >> factor) | (char_('+') >> factor);

BOOST_SPIRIT_DEFINE(expression, term, factor);

parser_type calculator() { return expression; }
}  // namespace calculator_grammar
}  // namespace client