#include "tcc-neo/parser/config.hpp"
#include "tcc-neo/parser/package_def.hpp"

namespace tcc {
parser::PackageDeclaration_type const& PackageDeclaration() { return parser::PackageDeclaration; }
namespace parser {
BOOST_SPIRIT_INSTANTIATE(PackageDeclaration_type, iterator_type, context_type);
}  // namespace parser
}  // namespace tcc