#include "fun/expression_def.hpp"
#include "fun/config.hpp"

// EXPRESSION_INSTANTIATE_VISIT_BEGIN
namespace fun { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(
        expression_type, iterator_type, context_type);
}}
// EXPRESSION_INSTANTIATE_VISIT_END
