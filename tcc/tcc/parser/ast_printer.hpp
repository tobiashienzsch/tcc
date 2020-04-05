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