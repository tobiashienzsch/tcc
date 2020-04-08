#pragma once

#include <vector>

#include "tcc/ir/three_address_code.hpp"

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(StatementScope const& scope) -> std::vector<int64_t>;
};
}  // namespace tcc