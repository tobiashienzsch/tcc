#include "tcc-neo/parser/package_def.hpp"

#include "tcc-neo/parser/config.hpp"

namespace tcc::parser
{
BOOST_SPIRIT_INSTANTIATE(PackageDeclaration_type, iterator_type, context_type);
}  // namespace tcc::parser
