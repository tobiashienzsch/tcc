#pragma once

#include "tcc/core/core.hpp"

#include "ir/three_address_code.hpp"

#include <map>
#include <vector>

namespace tcc
{
struct IntermediateRepresentation
{
    auto PushToStack(int x) -> void;
    auto PopFromStack() -> std::variant<int, std::string>;

    auto AddVariable(std::string name) -> void;
    auto GetLastVariable(std::string const& key) const -> std::string;

    auto CreateBinaryOperation(byte_code op) -> void;
    auto CreateUnaryOperation(byte_code op) -> void;
    auto CreateStoreOperation(std::string key) -> void;
    auto CreateLoadOperation(std::string key) -> void;
    auto CreateAssignment(std::string const& key) -> std::string;
    auto CreateTemporaryOnStack() -> std::string;

    void PrintStatementList() const;

private:
    int m_varCounter = 0;
    std::map<std::string, int> m_variables;
    std::vector<std::variant<int, std::string>> m_stack;
    std::vector<ThreeAddressCode> m_statements;
};
}  // namespace tcc