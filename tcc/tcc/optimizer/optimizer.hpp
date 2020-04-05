#pragma once

#include <algorithm>

#include "tcc/ir/intermediate_representation.hpp"
#include "tsl/tsl.hpp"

namespace tcc {

class Optimizer {
 public:
  Optimizer(StatementScope& scope) : m_mainScope(scope) {}

  auto PrintIR(std::string prefix) -> void;
  auto Optimize() -> void;

  static auto DeleteUnusedStatements(StatementList& statementList) -> bool;
  static auto IsUnusedStatement(ThreeAddressCode const& statement, StatementList const& statementList) -> bool;
  static auto ReplaceVariableIfConstant(ThreeAddressCode& statement, StatementList& statementList) -> bool;

  // replace constant expression with store of result.
  static auto ReplaceWithConstantStore(ThreeAddressCode& statement) -> bool;
  static auto isConstantArgument(ThreeAddressCode::Argument const& argument) -> bool;
  static auto isConstantArgument(ThreeAddressCode::OptionalArgument const& argument) -> bool;
  static auto isConstantStoreExpression(ThreeAddressCode const& statement) -> bool;
  static auto isConstantBinaryExpression(ThreeAddressCode const& statement) -> bool;
  static auto isBinaryOperation(byte_code op) noexcept -> bool;

 private:
  StatementScope& m_mainScope;
};
}  // namespace tcc