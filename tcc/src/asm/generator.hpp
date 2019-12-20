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

        // OPTIMIZE
        for (ThreeAddressCode& x : m_ir)
        {
            if (isBinaryOperation(x.type))
            {
                if (isConstant(x.first) && isConstant(x.second))
                {
                    x.first  = std::get<int>(x.first) + std::get<int>(x.second.value());
                    x.second = std::nullopt;
                    x.type   = byte_code::op_store;
                }
            }
        }

        fmt::print("\n\nAfter: {} lines\n", m_ir.size());
        for (ThreeAddressCode const& x : m_ir) std::cout << x << '\n';
    }

private:
    auto isConstant(ThreeAddressCode::Argument const& argument) const -> bool
    {
        auto returnValue = bool {false};
        std::visit(tcc::overloaded {
                       [&returnValue](int) { returnValue = true; },
                       [&returnValue](const std::string&) { returnValue = false; },
                   },
                   argument);

        return returnValue;
    }

    auto isConstant(ThreeAddressCode::OptionalArgument const& argument) const -> bool
    {
        if (argument.has_value()) return isConstant(argument.value());
        return false;
    }

    auto isBinaryOperation(byte_code op) const -> bool
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