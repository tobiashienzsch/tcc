#include "config.hpp"
#include "statement_def.hpp"

namespace tcc::parser
{
BOOST_SPIRIT_INSTANTIATE(statement_type, iterator_type, context_type);
}  // namespace tcc::parser
