#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "tcc/ir/intermediate_representation.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tsl/tsl.hpp"

namespace tcc {
namespace code_gen {

struct program {
  auto op(int a) -> void;
  auto op(int a, int b) -> void;

  auto operator[](std::size_t i) -> int& { return code[i]; }
  auto operator[](std::size_t i) const -> int { return code[i]; }

  auto clear() -> void {
    code.clear();
    variables.clear();
  }

  auto size() const -> std::size_t { return code.size(); }
  auto operator()() const -> std::vector<int> const& { return code; }

  auto nvars() const -> std::size_t { return variables.size(); }
  auto find_var(std::string const& name) const -> int const*;
  auto add_var(std::string const& name) -> void;

  auto print_variables(std::vector<int> const& stack) const -> void;

 private:
  std::map<std::string, int> variables;
  std::vector<int> code;
};

////////////////////////////////////////////////////////////////////////////
//  The Compiler
////////////////////////////////////////////////////////////////////////////
struct compiler {
  using result_type = bool;
  using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

  template <typename ErrorHandler>
  compiler(tcc::code_gen::program& prog, tcc::IntermediateRepresentation& builder, ErrorHandler const& errorHandler)
      : program(prog),
        m_builder(builder),
        error_handler([&](x3::position_tagged pos, std::string const& msg) { errorHandler(pos, msg); }) {}

  bool operator()(ast::Nil) const { return false; }
  bool operator()(uint64_t x) const;
  bool operator()(bool x) const;
  bool operator()(ast::Variable const& x) const;
  bool operator()(ast::Operation const& x) const;
  bool operator()(ast::Unary const& x) const;
  bool operator()(ast::Expression const& x) const;
  bool operator()(ast::Assignment const& x) const;
  bool operator()(ast::VariableDeclaration const& x) const;
  bool operator()(ast::StatementList const& x) const;
  bool operator()(ast::Statement const& x) const;
  bool operator()(ast::IfStatement const& x) const;
  bool operator()(ast::WhileStatement const& x) const;
  bool operator()(ast::ReturnStatement const& x) const;
  bool operator()(ast::CompoundStatement const&) const { return false; }

  bool start(ast::StatementList const& x) const;

  tcc::code_gen::program& program;
  tcc::IntermediateRepresentation& m_builder;
  error_handler_type error_handler = {};
};
}  // namespace code_gen
}  // namespace tcc
