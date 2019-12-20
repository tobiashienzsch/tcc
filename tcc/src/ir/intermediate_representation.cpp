#include "ir/intermediate_representation.hpp"

namespace tcc
{

auto IntermediateRepresentation::PushToStack(int x) -> void { m_stack.push_back(x); }

auto IntermediateRepresentation::PopFromStack() -> std::variant<int, std::string>
{
    auto const result = m_stack.back();
    m_stack.pop_back();
    return result;
}

auto IntermediateRepresentation::AddVariable(std::string name) -> void
{
    auto search = m_variables.find(name);
    if (search == m_variables.end())
        m_variables.insert({name, 0});
    else
        fmt::print("Tried to add {} twice to variable map\n", name);
}

auto IntermediateRepresentation::CreateBinaryOperation(byte_code op) -> void
{
    auto const second  = PopFromStack();
    auto const first   = PopFromStack();
    auto const tmpName = CreateTemporaryOnStack();

    m_statements.push_back(ThreeAddressCode {op, tmpName, first, second});
}

auto IntermediateRepresentation::CreateUnaryOperation(byte_code op) -> void
{
    auto const first   = PopFromStack();
    auto const tmpName = CreateTemporaryOnStack();
    m_statements.push_back(ThreeAddressCode {op, tmpName, first, {}});
}

auto IntermediateRepresentation::CreateStoreOperation(std::string key) -> void
{
    auto const first = PopFromStack();
    m_statements.push_back(ThreeAddressCode {op_store, key, first, {}});
}

auto IntermediateRepresentation::CreateLoadOperation(std::string key) -> void
{
    auto const tmpName = CreateTemporaryOnStack();
    m_statements.push_back(ThreeAddressCode {op_load, tmpName, key, {}});
}

auto IntermediateRepresentation::CreateAssignment(std::string const& key) -> std::string
{
    auto search = m_variables.find(key);
    auto newId  = search->second++;
    return fmt::format("{}{}", key, newId);
}

auto IntermediateRepresentation::GetLastVariable(std::string const& key) const -> std::string
{
    auto search = m_variables.find(key);
    auto newId  = search->second - 1;
    return fmt::format("{}{}", key, newId);
}

void IntermediateRepresentation::PrintStatementList() const
{
    fmt::print("#locals: {}\n", m_variables.size());

    for (auto const& x : m_statements)
    {
        std::cout << x << '\n';
    }

    fmt::print("-------\n\n", m_variables.size());
}

auto IntermediateRepresentation::CreateTemporaryOnStack() -> std::string
{
    auto tmp = std::string("t").append(std::to_string(m_varCounter++));
    m_stack.push_back(tmp);
    return tmp;
}

}  // namespace tcc