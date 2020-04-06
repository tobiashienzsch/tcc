#if !defined(TCC_PARSER_QI_COMPILER_HPP)
#define TCC_PARSER_QI_COMPILER_HPP

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <map>
#include <vector>

#include "tcc/parser-qi/ast.hpp"
#include "tcc/parser-qi/error_handler.hpp"

namespace client {
namespace code_gen {

struct function {
  function(std::vector<int>& code, int nargs) : code(code), address(code.size()), size_(0), nargs_(nargs) {}

  void op(int a);
  void op(int a, int b);
  void op(int a, int b, int c);

  int& operator[](std::size_t i) { return code[address + i]; }
  int const& operator[](std::size_t i) const { return code[address + i]; }
  std::size_t size() const { return size_; }
  std::size_t get_address() const { return address; }

  int nargs() const { return nargs_; }
  int nvars() const { return variables.size(); }
  int const* find_var(std::string const& name) const;
  void add_var(std::string const& name);
  void link_to(std::string const& name, std::size_t address);

  void print_assembler() const;

 private:
  std::map<std::string, int> variables;
  std::map<std::size_t, std::string> functionCalls;
  std::vector<int>& code;
  std::size_t address;
  std::size_t size_;
  std::size_t nargs_;
};

struct compiler {
  using result_type = bool;

  template <typename ErrorHandler>
  compiler(ErrorHandler& errorHandler) {
    using namespace boost::phoenix::arg_names;
    namespace phx = boost::phoenix;
    using boost::phoenix::function;

    errorHandler_ = function<ErrorHandler>(errorHandler)("Error! ", _2, phx::cref(errorHandler.iters)[_1]);
  }

  bool operator()(ast::Nil) {
    assert(false);
    return false;
  }
  bool operator()(unsigned int x);
  bool operator()(bool x);
  bool operator()(ast::Identifier const& x);
  bool operator()(ast::operation const& x);
  bool operator()(ast::Unary const& x);
  bool operator()(ast::FunctionCall const& x);
  bool operator()(ast::expression const& x);
  bool operator()(ast::Assignment const& x);
  bool operator()(ast::VariableDeclaration const& x);
  bool operator()(ast::StatementList const& x);
  bool operator()(ast::Statement const& x);
  bool operator()(ast::IfStatement const& x);
  bool operator()(ast::WhileStatement const& x);
  bool operator()(ast::ReturnStatement const& x);
  bool operator()(ast::function const& x);
  bool operator()(ast::FunctionList const& x);

  void print_assembler() const;

  boost::shared_ptr<code_gen::function> find_function(std::string const& name) const;

  std::vector<int>& get_code() { return code; }
  std::vector<int> const& get_code() const { return code; }

 private:
  using function_table = std::map<std::string, boost::shared_ptr<code_gen::function>>;

  std::vector<int> code{};
  code_gen::function* current{};
  std::string current_function_name{};
  function_table functions{};
  bool void_return{};

  boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace code_gen
}  // namespace client

#endif
