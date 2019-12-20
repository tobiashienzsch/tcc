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
    AssemblyGenerator(StatementList ir) : m_statementList(std::move(ir)) {}

    auto Generate() -> void
    {
        fmt::print("Before: {} lines\n", m_statementList.size());
        for (ThreeAddressCode const& x : m_statementList) std::cout << x << '\n';

        Optimize();

        fmt::print("\n\nAfter: {} lines\n", m_statementList.size());
        for (ThreeAddressCode const& x : m_statementList) std::cout << x << '\n';
    }

    auto Optimize() -> void
    {
        for (auto x : {1, 2})
        {
            std::for_each(std::begin(m_statementList), std::end(m_statementList),
                          [](auto& statement) { ReplaceWithConstantStore(statement); });

            std::for_each(std::begin(m_statementList), std::end(m_statementList),
                          [&](auto& statement) { ReplaceVariableIfConstant(statement, m_statementList); });
        }

        // DeleteUnusedStatements(m_statementList);
    }

    static auto DeleteUnusedStatements(StatementList& statementList) -> bool
    {
        while (true)
        {
            auto const elementToDelete = std::find_if(
                std::begin(statementList), std::end(statementList),
                [&statementList](auto const& statement) { return IsUnusedStatement(statement, statementList); });

            if (elementToDelete == std::end(statementList)) return false;

            statementList.erase(elementToDelete);
        }

        return false;
    }

    static auto IsUnusedStatement(ThreeAddressCode const& statement, StatementList const& statementList) -> bool
    {
        return !std::any_of(std::begin(statementList), std::end(statementList),
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

    static auto ReplaceVariableIfConstant(ThreeAddressCode& statement, StatementList& statementList) -> bool
    {
        if (isConstantStoreExpression(statement))
        {
            for (auto& otherStatement : statementList)
            {
                if (otherStatement.type != byte_code::op_load)
                {
                    // first
                    std::visit(tcc::overloaded {
                                   [](int) { ; },
                                   [&statement, &otherStatement](std::string const& name) {
                                       if (name == statement.destination)
                                       {
                                           otherStatement.first = std::get<int>(statement.first);
                                       };
                                   },
                               },
                               otherStatement.first);

                    // second
                    if (otherStatement.second.has_value())
                    {
                        std::visit(tcc::overloaded {
                                       [](int) { ; },
                                       [&statement, &otherStatement](std::string const& name) {
                                           if (name == statement.destination)
                                           {
                                               otherStatement.second = std::get<int>(statement.first);
                                           };
                                       },
                                   },
                                   otherStatement.second.value());
                    }
                }
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
    StatementList m_statementList;
};
}  // namespace tcc