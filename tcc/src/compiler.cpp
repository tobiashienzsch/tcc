#include "compiler.hpp"

#include <boost/assert.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <iostream>
#include <set>

#include "vm.hpp"

namespace tcc::code_gen {
auto program::op(int a) -> void { code.push_back(a); }

auto program::op(int a, int b) -> void {
  code.push_back(a);
  code.push_back(b);
}

auto program::find_var(std::string const& name) const -> int const* {
  auto i = variables.find(name);
  if (i == variables.end()) return nullptr;
  return &i->second;
}

auto program::add_var(std::string const& name) -> void {
  std::size_t n = variables.size();
  variables[name] = int(n);
}

auto program::print_variables(std::vector<int> const& stack) const -> void {
  for (auto const& p : variables) {
    std::cout << "    " << p.first << ": " << stack[p.second] << std::endl;
  }
}

auto compiler::operator()(uint64_t x) const -> bool {
  program.op(op_int, x);
  m_builder.PushToStack(x);
  return true;
}

auto compiler::operator()(bool x) const -> bool {
  program.op(x ? op_true : op_false);
  return true;
}

auto compiler::operator()(ast::variable const& x) const -> bool {
  int const* p = program.find_var(x.name);
  if (p == nullptr) {
    error_handler(x, "Undeclared variable: " + x.name);
    return false;
  }
  program.op(op_load, *p);
  auto const last = m_builder.GetLastVariable(x.name);
  m_builder.CreateLoadOperation(last);
  return true;
}

auto compiler::operator()(ast::operation const& x) const -> bool {
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::op_plus: {
      program.op(op_add);
      m_builder.CreateBinaryOperation(op_add);
      break;
    }
    case ast::op_minus: {
      program.op(op_sub);
      m_builder.CreateBinaryOperation(op_sub);
      break;
    }
    case ast::op_times: {
      program.op(op_mul);
      m_builder.CreateBinaryOperation(op_mul);
      break;
    }
    case ast::op_divide: {
      program.op(op_div);
      m_builder.CreateBinaryOperation(op_div);
      break;
    }

    case ast::op_equal: {
      program.op(op_eq);
      m_builder.CreateBinaryOperation(op_eq);
      break;
    }
    case ast::op_not_equal: {
      program.op(op_neq);
      m_builder.CreateBinaryOperation(op_neq);
      break;
    }
    case ast::op_less: {
      program.op(op_lt);
      m_builder.CreateBinaryOperation(op_lt);
      break;
    }
    case ast::op_less_equal: {
      program.op(op_lte);
      m_builder.CreateBinaryOperation(op_lte);
      break;
    }
    case ast::op_greater: {
      program.op(op_gt);
      m_builder.CreateBinaryOperation(op_gt);
      break;
    }
    case ast::op_greater_equal: {
      program.op(op_gte);
      m_builder.CreateBinaryOperation(op_gte);
      break;
    }

    case ast::op_and: {
      program.op(op_and);
      m_builder.CreateBinaryOperation(op_and);
      break;
    }
    case ast::op_or: {
      program.op(op_or);
      m_builder.CreateBinaryOperation(op_or);
      break;
    }
    default:
      return false;
  }
  return true;
}

auto compiler::operator()(ast::Unary const& x) const -> bool {
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::op_positive:
      break;

    case ast::op_negative: {
      program.op(op_neg);
      m_builder.CreateUnaryOperation(op_neg);
      break;
    }
    case ast::op_not: {
      program.op(op_not);
      m_builder.CreateUnaryOperation(op_not);
      break;
    }

    default:
      return false;
  }
  return true;
}

auto compiler::operator()(ast::Expression const& x) const -> bool {
  if (!boost::apply_visitor(*this, x.first)) return false;
  for (ast::operation const& oper : x.rest) {
    if (!(*this)(oper)) return false;
  }
  return true;
}

auto compiler::operator()(ast::assignment const& x) const -> bool {
  if (!(*this)(x.rhs)) return false;
  int const* p = program.find_var(x.lhs.name);
  if (p == nullptr) {
    error_handler(x.lhs, "Undeclared variable: " + x.lhs.name);
    return false;
  }
  program.op(op_store, *p);
  auto const newKey = m_builder.CreateAssignment(x.lhs.name);
  m_builder.CreateStoreOperation(newKey);
  return true;
}

auto compiler::operator()(ast::variable_declaration const& x) const -> bool {
  int const* p = program.find_var(x.assign.lhs.name);
  if (p != nullptr) {
    error_handler(x.assign.lhs, "Duplicate variable: " + x.assign.lhs.name);
    return false;
  }
  bool r = (*this)(x.assign.rhs);
  if (r)  // don't add the variable if the RHS fails
  {
    m_builder.AddVariable(x.assign.lhs.name);
    auto const newKey = m_builder.CreateAssignment(x.assign.lhs.name);
    m_builder.CreateStoreOperation(newKey);
    program.add_var(x.assign.lhs.name);
    program.op(op_store, *program.find_var(x.assign.lhs.name));
  }
  return r;
}

auto compiler::operator()(ast::Statement const& x) const -> bool { return boost::apply_visitor(*this, x); }

auto compiler::operator()(ast::StatementList const& x) const -> bool {
  for (auto const& s : x) {
    if (!(*this)(s)) return false;
  }
  return true;
}

auto compiler::operator()(ast::IfStatement const& x) const -> bool {
  if (!(*this)(x.condition)) return false;
  program.op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t skip = program.size() - 1;  // mark its position
  if (!(*this)(x.then)) return false;
  program[skip] = int(program.size() - skip);  // now we know where to jump to (after the if branch)

  if (x.else_)  // We got an alse
  {
    program[skip] += 2;                     // adjust for the "else" jump
    program.op(op_jump, 0);                 // we shall fill this (0) in later
    std::size_t exit = program.size() - 1;  // mark its position
    if (!(*this)(*x.else_)) return false;
    program[exit] = int(program.size() - exit);  // now we know where to jump to (after the else branch)
  }

  return true;
}

auto compiler::operator()(ast::WhileStatement const& x) const -> bool {
  std::size_t loop = program.size();  // mark our position
  if (!(*this)(x.condition)) return false;
  program.op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t exit = program.size() - 1;  // mark its position
  if (!(*this)(x.body)) return false;
  program.op(op_jump,
             int(loop - 1) - int(program.size()));  // loop back
  program[exit] = int(program.size() - exit);       // now we know where to jump to (to exit the loop)
  return true;
}

auto compiler::start(ast::StatementList const& x) const -> bool {
  program.clear();
  // op_stk_adj 0 for now. we'll know how many variables we'll have later
  program.op(op_stk_adj, 0);

  if (!(*this)(x)) {
    program.clear();
    return false;
  }
  program[1] = int(program.nvars());  // now store the actual number of variables
  return true;
}
}  // namespace tcc::code_gen
