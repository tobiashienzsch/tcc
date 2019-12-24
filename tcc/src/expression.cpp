#include "config.hpp"
#include "expression_def.hpp"

namespace tcc::parser
{
BOOST_SPIRIT_INSTANTIATE(expression_type, iterator_type, context_type);
}  // namespace tcc::parser
