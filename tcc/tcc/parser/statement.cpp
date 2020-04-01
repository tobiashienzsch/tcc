#include "tcc/parser/config.hpp"
#include "tcc/parser/statement_def.hpp"

namespace tcc::parser {
BOOST_SPIRIT_INSTANTIATE(Statement_type, iterator_type, context_type);
}  // namespace tcc::parser
