#pragma once

#include <map>
#include <vector>

#include "tcc/ir/three_address_code.hpp"
#include "tcc/core.hpp"

namespace tcc {
struct IntermediateRepresentation {
  IntermediateRepresentation() : m_currentScope(&m_mainScope) {}

  auto CurrentScope() -> StatementScope* {
    if (!m_currentScope) {
      fmt::print("Current scope is nullptr;\n EXIT\n");
      std::exit(1);
    }

    return m_currentScope;
  }

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

  auto GetStatementList() -> StatementList& { return m_mainScope.statements; }

 private:
  int m_varCounter = 0;
  std::vector<std::variant<int, std::string>> m_stack;
  StatementScope m_mainScope{"main"};
  StatementScope* m_currentScope = nullptr;
};

}  // namespace tcc