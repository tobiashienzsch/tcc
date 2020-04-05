#pragma once

#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/vm.hpp"

namespace tcc::parser {
class AstPrinter {
 public:
  using result_type = bool;
  using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

 public:
  template <typename ErrorHandler>
  AstPrinter(ErrorHandler const& errorHandler)
      : error_handler([&](x3::position_tagged pos, std::string const& msg) { errorHandler(pos, msg); }) {}

  bool start(ast::StatementList const& x);

  bool operator()(ast::Nil) const { return false; }
  bool operator()(uint64_t x);
  bool operator()(bool x);
  bool operator()(ast::Variable const& x);
  bool operator()(ast::Operation const& x);
  bool operator()(ast::Unary const& x);
  bool operator()(ast::Expression const& x);
  bool operator()(ast::Assignment const& x);
  bool operator()(ast::VariableDeclaration const& x);
  bool operator()(ast::StatementList const& x);
  bool operator()(ast::Statement const& x);
  bool operator()(ast::IfStatement const& x);
  bool operator()(ast::WhileStatement const& x);
  bool operator()(ast::ReturnStatement const& x);
  bool operator()(ast::CompoundStatement const& x) { return false; }

 private:
  auto pushToStack(int x) -> void { _stack.emplace_back(x); }

  auto popFromStack() -> int {
    auto const result = _stack.back();
    _stack.pop_back();
    return result;
  }

  auto AddVariable(std::string name) -> void {
    // auto search = m_mainScope.variables.find(name);
    // if (search == m_mainScope.variables.end())
    //   m_mainScope.variables.insert({name, 0});
    // else
    //   fmt::print("Tried to add {} twice to variable map\n", name);
  }

  auto createBinaryOperation(byte_code op) -> std::string {
    // auto const second = popFromStack();
    // auto const first = popFromStack();

    return fmt::format("{}", op);
  }

  auto createUnaryOperation(byte_code op) -> void {
    auto const first = popFromStack();
    fmt::print("<Unary op=\"{}\" first=\"{}\">\n", op, first);
  }

  auto createStoreOperation(std::string key) -> void {
    // auto const first = popFromStack();
    // m_mainScope.statements.push_back(ThreeAddressCode{op_store, key, first, {}, false});
  }

  auto createLoadOperation(std::string key) -> void {
    // auto const tmpName = createTemporaryOnStack();
    // m_mainScope.statements.push_back(ThreeAddressCode{op_load, tmpName, key, {}});
  }

  auto createAssignment(std::string const& key) -> std::string { fmt::print("<Assignment var={}>\n", key); }

  // auto GetLastVariable(std::string const& key) const -> std::string {
  //   auto search = m_mainScope.variables.find(key);
  //   auto newId = search->second - 1;
  //   return fmt::format("{}.{}", key, newId);
  // }

  auto printReturnOperation() -> void {
    printIndentation();
    fmt::print("</Return>\n");
  }

  auto printIndentation() const -> void {
    for (auto i = 0; i < _identationLevel; i++) {
      fmt::print("  ");
    }
  }

 private:
  std::vector<int> _stack = {};
  int _identationLevel = 0;
  error_handler_type error_handler;
};
}  // namespace tcc::parser