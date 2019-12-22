#include "ast.hpp"

namespace tcc
{

int64_t AppendExpression(InstructionList& dest, Expression const& source)
{
    auto const assembly = source.GetAssembly();
    for (auto const& x : assembly) dest.push_back(x);

    return static_cast<int64_t>(assembly.size());
}
}  // namespace tcc