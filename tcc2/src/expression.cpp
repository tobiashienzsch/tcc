#include "fun/config.hpp"
#include "fun/expression_def.hpp"

// EXPRESSION_INSTANTIATE_VISIT_BEGIN
namespace fun
{
namespace parser
{
BOOST_SPIRIT_INSTANTIATE(expression_type, iterator_type, context_type);
}
}  // namespace fun
// EXPRESSION_INSTANTIATE_VISIT_END
