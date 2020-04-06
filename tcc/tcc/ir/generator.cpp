#include "tcc/ir/generator.hpp"

#include <boost/variant/apply_visitor.hpp>
#include <cassert>

namespace tcc {

auto Program::op(int a) -> void { code.push_back(a); }

auto Program::op(int a, int b) -> void {
  code.push_back(a);
  code.push_back(b);
}

auto Program::find_var(std::string const& name) const -> int const* {
  auto i = variables.find(name);
  if (i == variables.end()) return nullptr;
  return &i->second;
}

auto Program::add_var(std::string const& name) -> void {
  std::size_t n = variables.size();
  variables[name] = int(n);
}

auto Program::print_variables(std::vector<int> const& stack) const -> void {
  for (auto const& p : variables) {
    std::cout << "    " << p.first << ": " << stack[p.second] << std::endl;
  }
}
bool IRGenerator::operator()(client::ast::Nil) {
  assert(false);
  return false;
}

bool IRGenerator::operator()(unsigned int x) { return true; }
bool IRGenerator::operator()(bool x) { return true; }
bool IRGenerator::operator()(client::ast::Identifier const& x) { return true; }
bool IRGenerator::operator()(client::ast::Operation const& x) { return true; }
bool IRGenerator::operator()(client::ast::Unary const& x) { return true; }
bool IRGenerator::operator()(client::ast::FunctionCall const& x) { return true; }
bool IRGenerator::operator()(client::ast::Expression const& x) { return true; }
bool IRGenerator::operator()(client::ast::Assignment const& x) { return true; }
bool IRGenerator::operator()(client::ast::VariableDeclaration const& x) { return true; }
bool IRGenerator::operator()(client::ast::Statement const& x) { return boost::apply_visitor(*this, x); }
bool IRGenerator::operator()(client::ast::StatementList const& x) { return true; }
bool IRGenerator::operator()(client::ast::IfStatement const& x) { return true; }
bool IRGenerator::operator()(client::ast::WhileStatement const& x) { return true; }
bool IRGenerator::operator()(client::ast::ReturnStatement const& x) { return true; }
bool IRGenerator::operator()(client::ast::function const& x) { return true; }
bool IRGenerator::operator()(client::ast::FunctionList const& x) { return true; }

}  // namespace tcc
