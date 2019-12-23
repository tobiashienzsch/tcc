#pragma once

#include "ir/three_address_code.hpp"

#include <vector>

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(StatementScope const& scope) -> std::vector<int64_t>;
};
}  // namespace tcc