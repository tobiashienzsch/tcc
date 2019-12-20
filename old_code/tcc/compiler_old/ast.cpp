#include "ast.hpp"

namespace tcc
{

Integer AppendExpression(InstructionList& dest, Expression const& source)
{
    auto const assembly = source.GetAssembly();
    for (auto const& x : assembly) dest.push_back(x);

    return static_cast<Integer>(assembly.size());
}
}  // namespace tcc