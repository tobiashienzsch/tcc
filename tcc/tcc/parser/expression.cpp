#include "tcc/parser/config.hpp"
#include "tcc/parser/expression_def.hpp"

namespace tcc::parser {
BOOST_SPIRIT_INSTANTIATE(Expression_type, iterator_type, context_type);
}  // namespace tcc::parser
