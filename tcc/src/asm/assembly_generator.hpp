#pragma once

#include "tcc/core/core.hpp"

#include "ir/three_address_code.hpp"

#include <algorithm>

namespace tcc
{
class AssemblyGenerator
{
public:
    static auto Build(StatementScope const& scope) -> void
    {
        auto const& statements = scope.statements;
        auto const numLocals   = scope.variables.size();
        fmt::print("\nNum Locals: {}\n", numLocals);

        auto localVars = std::vector<std::string> {};
        for (auto const& var : scope.variables)
        {
            localVars.push_back(var.first);
            fmt::print("ICONST, 0\n");
        }

        for (ThreeAddressCode const& statement : statements)
        {
            switch (statement.type)
            {
                case byte_code::op_store:
                {
                    if (auto* value = std::get_if<int>(&statement.first); value != nullptr)
                    {
                        fmt::print("ICONST, {}\n", *value);
                    }
                    auto const destIter  = std::find(std::begin(localVars), std::end(localVars),
                                                    std::string(1, statement.destination[0]));
                    auto const destIndex = static_cast<int>(destIter - std::begin(localVars));
                    fmt::print("STORE, {}\n", destIndex);
                    break;
                }

                case byte_code::op_load:
                {

                    auto const destIter  = std::find(std::begin(localVars), std::end(localVars),
                                                    std::string(1, std::get<std::string>(statement.first)[0]));
                    auto const destIndex = static_cast<int>(destIter - std::begin(localVars));
                    fmt::print("LOAD, {}\n", destIndex);
                    break;
                }

                case byte_code::op_add: fmt::print("ADD,\n"); break;
                case byte_code::op_sub: fmt::print("SUB,\n"); break;
                case byte_code::op_mul: fmt::print("MUL,\n"); break;
                case byte_code::op_div: fmt::print("DIV,\n"); break;

                default: break;
            }
        }
    }
};
}  // namespace tcc