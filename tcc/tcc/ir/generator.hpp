#if !defined(TCC_IR_GENERATOR_HPP)
#define TCC_IR_GENERATOR_HPP

#include <boost/function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "tcc/ir/three_address_code.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"

namespace tcc {

class IRGenerator {
 public:
  using result_type = bool;

  template <typename ErrorHandler>
  IRGenerator(ErrorHandler& errorHandler) {
    using namespace boost::phoenix::arg_names;
    namespace phx = boost::phoenix;
    using boost::phoenix::function;

    errorHandler_ = function<ErrorHandler>(errorHandler)("Error! ", _2, phx::cref(errorHandler.iters)[_1]);
  }

  bool operator()(unsigned int x);
  bool operator()(bool x);
  bool operator()(tcc::ast::Nil);
  bool operator()(tcc::ast::Identifier const& x);
  bool operator()(tcc::ast::Operation const& x);
  bool operator()(tcc::ast::Unary const& x);
  bool operator()(tcc::ast::FunctionCall const& x);
  bool operator()(tcc::ast::Expression const& x);
  bool operator()(tcc::ast::Assignment const& x);
  bool operator()(tcc::ast::VariableDeclaration const& x);
  bool operator()(tcc::ast::StatementList const& x);
  bool operator()(tcc::ast::Statement const& x);
  bool operator()(tcc::ast::IfStatement const& x);
  bool operator()(tcc::ast::WhileStatement const& x);
  bool operator()(tcc::ast::ReturnStatement const& x);
  bool operator()(tcc::ast::Function const& x);
  bool operator()(tcc::ast::FunctionList const& x);

  auto PrintIR() -> void { builder_.PrintIR(); }
  auto CurrentScope() -> StatementScope* { return builder_.CurrentScope(); }

 private:
  struct Builder {
    Builder() = default;

    auto PrintIR() -> void {
      fmt::print("\nprogram: {} IR instructions\n", m_mainScope.statements.size());
      fmt::print("func main: args=[]\n");
      fmt::print("entry:\n");
      for (ThreeAddressCode const& x : m_mainScope.statements) fmt::print("\t{}\n", x);
    }

    auto CurrentScope() -> StatementScope* {
      if (!m_currentScope) {
        fmt::print("Current scope is nullptr;\n EXIT\n");
        std::exit(1);
      }

      return m_currentScope;
    }

    auto HasVariable(std::string const& name) const -> bool {
      auto i = m_mainScope.variables.find(name);
      if (i == m_mainScope.variables.end()) return false;
      return true;
    }

    auto PushToStack(int x) -> void { m_stack.emplace_back(x); }

    auto PopFromStack() -> std::variant<int, std::string> {
      auto const result = m_stack.back();
      m_stack.pop_back();
      return result;
    }

    auto AddVariable(std::string name) -> void {
      auto search = m_mainScope.variables.find(name);
      if (search == m_mainScope.variables.end())
        m_mainScope.variables.insert({name, 0});
      else
        fmt::print("Tried to add {} twice to variable map\n", name);
    }

    auto GetLastVariable(std::string const& key) const -> std::string {
      auto search = m_mainScope.variables.find(key);
      auto newId = search->second - 1;
      return fmt::format("{}.{}", key, newId);
    }

    auto CreateReturnOperation() -> void {
      auto const first = PopFromStack();
      m_mainScope.statements.push_back(ThreeAddressCode{byte_code::op_return, "g.0", first, std::nullopt, false});
    }

    auto CreateBinaryOperation(byte_code op) -> void {
      auto const second = PopFromStack();
      auto const first = PopFromStack();
      auto const tmpName = CreateTemporaryOnStack();

      m_mainScope.statements.push_back(ThreeAddressCode{op, tmpName, first, second});
    }

    auto CreateUnaryOperation(byte_code op) -> void {
      auto const first = PopFromStack();
      auto const tmpName = CreateTemporaryOnStack();
      m_mainScope.statements.push_back(ThreeAddressCode{op, tmpName, first, {}});
    }

    auto CreateStoreOperation(std::string key) -> void {
      auto const first = PopFromStack();
      m_mainScope.statements.push_back(ThreeAddressCode{op_store, key, first, {}, false});
    }

    auto CreateLoadOperation(std::string key) -> void {
      auto const tmpName = CreateTemporaryOnStack();
      m_mainScope.statements.push_back(ThreeAddressCode{op_load, tmpName, key, {}});
    }

    auto CreateAssignment(std::string const& key) -> std::string {
      auto search = m_mainScope.variables.find(key);
      auto newId = search->second++;
      return fmt::format("{}.{}", key, newId);
    }

    auto CreateTemporaryOnStack() -> std::string {
      auto tmp = std::string("t.").append(std::to_string(m_varCounter++));
      m_stack.emplace_back(tmp);
      return tmp;
    }

    auto GetStatementList() -> StatementList& { return m_mainScope.statements; }

   private:
    int m_varCounter = 0;
    std::vector<std::variant<int, std::string>> m_stack;
    StatementScope m_mainScope{"main"};
    StatementScope* m_currentScope = &m_mainScope;
  };

 private:
  Builder builder_{};
  boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
