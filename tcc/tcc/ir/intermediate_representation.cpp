#include "tcc/ir/intermediate_representation.hpp"

namespace tcc {

auto IntermediateRepresentation::PushToStack(int x) -> void { m_stack.emplace_back(x); }

auto IntermediateRepresentation::PopFromStack() -> std::variant<int, std::string> {
  auto const result = m_stack.back();
  m_stack.pop_back();
  return result;
}

auto IntermediateRepresentation::AddVariable(std::string name) -> void {
  auto search = m_mainScope.variables.find(name);
  if (search == m_mainScope.variables.end())
    m_mainScope.variables.insert({name, 0});
  else
    fmt::print("Tried to add {} twice to variable map\n", name);
}

auto IntermediateRepresentation::CreateReturnOperation() -> void {
  auto const first = PopFromStack();
  m_mainScope.statements.push_back(ThreeAddressCode{byte_code::op_return, "g.0", first, std::nullopt, false});
}

auto IntermediateRepresentation::CreateBinaryOperation(byte_code op) -> void {
  auto const second = PopFromStack();
  auto const first = PopFromStack();
  auto const tmpName = CreateTemporaryOnStack();

  m_mainScope.statements.push_back(ThreeAddressCode{op, tmpName, first, second});
}

auto IntermediateRepresentation::CreateUnaryOperation(byte_code op) -> void {
  auto const first = PopFromStack();
  auto const tmpName = CreateTemporaryOnStack();
  m_mainScope.statements.push_back(ThreeAddressCode{op, tmpName, first, {}});
}

auto IntermediateRepresentation::CreateStoreOperation(std::string key) -> void {
  auto const first = PopFromStack();
  m_mainScope.statements.push_back(ThreeAddressCode{op_store, key, first, {}, false});
}

auto IntermediateRepresentation::CreateLoadOperation(std::string key) -> void {
  auto const tmpName = CreateTemporaryOnStack();
  m_mainScope.statements.push_back(ThreeAddressCode{op_load, tmpName, key, {}});
}

auto IntermediateRepresentation::CreateAssignment(std::string const& key) -> std::string {
  auto search = m_mainScope.variables.find(key);
  auto newId = search->second++;
  return fmt::format("{}.{}", key, newId);
}

auto IntermediateRepresentation::GetLastVariable(std::string const& key) const -> std::string {
  auto search = m_mainScope.variables.find(key);
  auto newId = search->second - 1;
  return fmt::format("{}.{}", key, newId);
}

auto IntermediateRepresentation::CreateTemporaryOnStack() -> std::string {
  auto tmp = std::string("t.").append(std::to_string(m_varCounter++));
  m_stack.emplace_back(tmp);
  return tmp;
}

}  // namespace tcc