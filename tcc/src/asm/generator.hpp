#pragma once

#include "tcc/core/core.hpp"

#include "ir/intermediate_representation.hpp"

#include <algorithm>

namespace tcc
{
using StatementList = std::vector<ThreeAddressCode>;

class AssemblyGenerator
{
public:
    AssemblyGenerator(StatementList ir) : m_ir(std::move(ir)) {}

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
        for (auto _ : {1, 2, 3})
        {
            tcc::IgnoreUnused(_);

            for (ThreeAddressCode& statement : m_ir)
            {
                ReplaceWithConstantStore(statement);
                ReplaceVariableWithConstant(statement);
            }
        }
        DeleteUnusedStatements(m_ir);
    }

    static auto DeleteUnusedStatements(StatementList& statementList) -> bool
    {
        std::remove_if(std::begin(statementList), std::end(statementList),
                       [&statementList](auto const& statement) { return IsUnusedStatement(statement, statementList); });
        return false;
    }

    static auto IsUnusedStatement(ThreeAddressCode const& statement, StatementList const& statementList) -> bool
    {
        return std::none_of(std::begin(statementList), std::end(statementList),
                            [&statement](ThreeAddressCode const& item) {
                                auto result = false;

                                std::visit(tcc::overloaded {
                                               [](int) { ; },
                                               [&statement, &result](std::string const& name) {
                                                   if (name == statement.destination) result = true;
                                               },
                                           },
                                           item.first);

                                if (item.second.has_value())
                                {
                                    std::visit(tcc::overloaded {
                                                   [](int) { ; },
                                                   [&statement, &result](std::string const& name) {
                                                       if (name == statement.destination) result = true;
                                                   },
                                               },
                                               item.second.value());
                                }

                                return result;
                            });
    }

    auto ReplaceVariableWithConstant(ThreeAddressCode& statement) -> bool
    {
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

            return true;
        }

        return false;
    }

    // replace constant expression with store of result.
    static auto ReplaceWithConstantStore(ThreeAddressCode& statement) -> bool
    {
        if (isConstantBinaryExpression(statement))
        {
            auto const first  = std::get<int>(statement.first);
            auto const second = std::get<int>(statement.second.value());

            switch (statement.type)
            {
                case byte_code::op_add: statement.first = first + second; break;
                case byte_code::op_sub: statement.first = first - second; break;
                case byte_code::op_mul: statement.first = first * second; break;
                case byte_code::op_div: statement.first = first / second; break;
                default: break;
            }

            statement.second = std::nullopt;
            statement.type   = byte_code::op_store;

            return true;
        }

        return false;
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
        if (argument.has_value())
        {
            return isConstantArgument(argument.value());
        }

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
    StatementList m_ir;
};
}  // namespace tcc