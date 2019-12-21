#pragma once

#include "tcc/core/core.hpp"

#include "ir/three_address_code.hpp"

#include <algorithm>

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(StatementList const& statements) -> void
    {
        auto const numLocals
            = std::count_if(std::begin(statements), std::end(statements),
                            [](ThreeAddressCode const& statement) { return statement.isTemporary == false; });

        fmt::print("\nNum Locals: {}\n", numLocals);
        for (auto x = 0; x < numLocals; x++) fmt::print("ICONST, 0\n");

        for (ThreeAddressCode const& statement : statements)
        {
            switch (statement.type)
            {
                case byte_code::op_store: fmt::print("ICONST, {}\nSTORE, {}", 42, 0); break;
                default: break;
            }
        }
    }
};
}  // namespace tcc