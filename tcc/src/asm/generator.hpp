#pragma once

#include "tcc/core/core.hpp"

#include "ir/intermediate_representation.hpp"

namespace tcc
{
class AssemblyGenerator
{
public:
    AssemblyGenerator(std::vector<ThreeAddressCode> ir) : m_ir(std::move(ir)) {}

    auto Generate() -> void
    {
        fmt::print("Before: {} lines\n", m_ir.size());
        for (ThreeAddressCode const& x : m_ir) std::cout << x << '\n';

        Optimize();

        fmt::print("\n\nAfter: {} lines\n", m_ir.size());
        for (ThreeAddressCode const& x : m_ir) std::cout << x << '\n';
    }

    auto Optimize() -> void
    {
        for (ThreeAddressCode& statement : m_ir)
        {
            // replace constant expression with store of result.
            if (isConstantBinaryExpression(statement))
            {
                statement.first  = std::get<int>(statement.first) + std::get<int>(statement.second.value());
                statement.second = std::nullopt;
                statement.type   = byte_code::op_store;
            }

            if (isConstantStoreExpression(statement))
            {
                for (auto& nextStatement : m_ir)
                {
                    // first
                    std::visit(tcc::overloaded {
                                   [](int) { ; },
                                   [&statement, &nextStatement](std::string const& name) {
                                       if (name == statement.destination)
                                       {
                                           fmt::print("{}", name);
                                           nextStatement.first = std::get<int>(statement.first);
                                       };
                                   },
                               },
                               nextStatement.first);
                }
            }
        }
    }

    static auto isConstantArgument(ThreeAddressCode::Argument const& argument) -> bool
    {
        auto returnValue = bool {false};
        std::visit(tcc::overloaded {
                       [&returnValue](int) { returnValue = true; },
                       [&returnValue](const std::string&) { returnValue = false; },
                   },
                   argument);

        return returnValue;
    }

    static auto isConstantArgument(ThreeAddressCode::OptionalArgument const& argument) -> bool
    {
        if (argument.has_value()) return isConstantArgument(argument.value());
        return false;
    }

    static auto isConstantStoreExpression(ThreeAddressCode const& statement) -> bool
    {
        if (statement.type == byte_code::op_store && isConstantArgument(statement.first))
        {
            return true;
        }
        return false;
    }

    static auto isConstantBinaryExpression(ThreeAddressCode const& statement) -> bool
    {
        if (isBinaryOperation(statement.type))
        {
            if (isConstantArgument(statement.first) && isConstantArgument(statement.second))
            {
                return true;
            }
        }

        return false;
    }

    static auto isBinaryOperation(byte_code op) -> bool
    {
        if (op == byte_code::op_add) return true;
        if (op == byte_code::op_sub) return true;
        if (op == byte_code::op_mul) return true;
        if (op == byte_code::op_div) return true;
        return false;
    }

private:
    std::vector<ThreeAddressCode> m_ir;
};
}  // namespace tcc