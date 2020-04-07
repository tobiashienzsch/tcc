#if !defined(TCC_IR_GENERATOR_HPP)
#define TCC_IR_GENERATOR_HPP

#include <boost/function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <map>
#include <string>
#include <vector>

#include "tcc/ir/intermediate_representation.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"

namespace tcc {

struct Program {
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

 private:
  std::map<std::string, int> variables;
  std::vector<int> code;
};

struct IRGenerator {
  using result_type = bool;

  template <typename ErrorHandler>
  IRGenerator(Program& prog, tcc::IntermediateRepresentation& builder, ErrorHandler& errorHandler)
      : program_(prog), builder_(builder) {
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

 private:
 private:
  Program& program_;
  tcc::IntermediateRepresentation& builder_;
  boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
