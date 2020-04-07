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

bool IRGenerator::operator()(tcc::ast::Nil) {
  assert(false);
  return false;
}

bool IRGenerator::operator()(unsigned int x) {
  program_.op(op_int, x);
  builder_.PushToStack(x);
  return true;
}
bool IRGenerator::operator()(bool x) {
  program_.op(x ? op_true : op_false);
  return true;
}
bool IRGenerator::operator()(tcc::ast::Identifier const& x) {
  int const* p = program_.find_var(x.name);
  if (p == nullptr) {
    errorHandler_(x.id, "Undeclared variable: " + x.name);
    return false;
  }
  program_.op(op_load, *p);
  auto const last = builder_.GetLastVariable(x.name);
  builder_.CreateLoadOperation(last);
  return true;
}

bool IRGenerator::operator()(tcc::ast::Operation const& x) {
  if (!boost::apply_visitor(*this, x.operand)) return false;
  switch (x.operator_) {
    case tcc::ast::OpToken::Plus: {
      program_.op(op_add);
      builder_.CreateBinaryOperation(op_add);
      break;
    }
    case tcc::ast::OpToken::Minus: {
      program_.op(op_sub);
      builder_.CreateBinaryOperation(op_sub);
      break;
    }
    case tcc::ast::OpToken::Times: {
      program_.op(op_mul);
      builder_.CreateBinaryOperation(op_mul);
      break;
    }
    case tcc::ast::OpToken::Divide: {
      program_.op(op_div);
      builder_.CreateBinaryOperation(op_div);
      break;
    }

    case tcc::ast::OpToken::Equal: {
      program_.op(op_eq);
      builder_.CreateBinaryOperation(op_eq);
      break;
    }
    case tcc::ast::OpToken::NotEqual: {
      program_.op(op_neq);
      builder_.CreateBinaryOperation(op_neq);
      break;
    }
    case tcc::ast::OpToken::Less: {
      program_.op(op_lt);
      builder_.CreateBinaryOperation(op_lt);
      break;
    }
    case tcc::ast::OpToken::LessEqual: {
      program_.op(op_lte);
      builder_.CreateBinaryOperation(op_lte);
      break;
    }
    case tcc::ast::OpToken::Greater: {
      program_.op(op_gt);
      builder_.CreateBinaryOperation(op_gt);
      break;
    }
    case tcc::ast::OpToken::GreaterEqual: {
      program_.op(op_gte);
      builder_.CreateBinaryOperation(op_gte);
      break;
    }

    case tcc::ast::OpToken::And: {
      program_.op(op_and);
      builder_.CreateBinaryOperation(op_and);
      break;
    }
    case tcc::ast::OpToken::Or: {
      program_.op(op_or);
      builder_.CreateBinaryOperation(op_or);
      break;
    }
    default:
      return false;
  }
  return true;
}
bool IRGenerator::operator()(tcc::ast::Unary const& x) {
  if (!boost::apply_visitor(*this, x.operand)) return false;
  switch (x.operator_) {
    case tcc::ast::OpToken::Positive:
      break;

    case tcc::ast::OpToken::Negative: {
      program_.op(op_neg);
      builder_.CreateUnaryOperation(op_neg);
      break;
    }
    case tcc::ast::OpToken::Not: {
      program_.op(op_not);
      builder_.CreateUnaryOperation(op_not);
      break;
    }

    default:
      return false;
  }
  return true;
}
bool IRGenerator::operator()(tcc::ast::FunctionCall const& x) { return true; }
bool IRGenerator::operator()(tcc::ast::Expression const& x) {
  if (!boost::apply_visitor(*this, x.first)) return false;
  for (tcc::ast::Operation const& oper : x.rest) {
    if (!(*this)(oper)) return false;
  }
  return true;
}
bool IRGenerator::operator()(tcc::ast::Assignment const& x) {
  if (!(*this)(x.rhs)) return false;
  int const* p = program_.find_var(x.lhs.name);
  if (p == nullptr) {
    errorHandler_(x.lhs.id, "Undeclared variable: " + x.lhs.name);
    return false;
  }
  program_.op(op_store, *p);
  auto const newKey = builder_.CreateAssignment(x.lhs.name);
  builder_.CreateStoreOperation(newKey);
  return true;
}

bool IRGenerator::operator()(tcc::ast::VariableDeclaration const& x) {
  int const* p = program_.find_var(x.lhs.name);
  if (p != nullptr) {
    errorHandler_(x.lhs.id, "Duplicate variable: " + x.lhs.name);
    return false;
  }
  bool r = (*this)(*x.rhs);
  if (r)  // don't add the variable if the RHS fails
  {
    builder_.AddVariable(x.lhs.name);
    auto const newKey = builder_.CreateAssignment(x.lhs.name);
    builder_.CreateStoreOperation(newKey);
    program_.add_var(x.lhs.name);
    program_.op(op_store, *program_.find_var(x.lhs.name));
  }
  return r;
}
bool IRGenerator::operator()(tcc::ast::Statement const& x) { return boost::apply_visitor(*this, x); }
bool IRGenerator::operator()(tcc::ast::StatementList const& x) {
  for (auto const& s : x) {
    if (!(*this)(s)) return false;
  }
  return true;
}
bool IRGenerator::operator()(tcc::ast::IfStatement const& x) {
  if (!(*this)(x.condition)) return false;
  program_.op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t skip = program_.size() - 1;  // mark its position
  if (!(*this)(x.then)) return false;
  program_[skip] = int(program_.size() - skip);  // now we know where to jump to (after the if branch)

  if (x.else_)  // We got an alse
  {
    program_[skip] += 2;                     // adjust for the "else" jump
    program_.op(op_jump, 0);                 // we shall fill this (0) in later
    std::size_t exit = program_.size() - 1;  // mark its position
    if (!(*this)(*x.else_)) return false;
    program_[exit] = int(program_.size() - exit);  // now we know where to jump to (after the else branch)
  }

  return true;
}
bool IRGenerator::operator()(tcc::ast::WhileStatement const& x) {
  std::size_t loop = program_.size();  // mark our position
  if (!(*this)(x.condition)) return false;
  program_.op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t exit = program_.size() - 1;  // mark its position
  if (!(*this)(x.body)) return false;
  program_.op(op_jump,
              int(loop - 1) - int(program_.size()));  // loop back
  program_[exit] = int(program_.size() - exit);       // now we know where to jump to (to exit the loop)
  return true;
}
bool IRGenerator::operator()(tcc::ast::ReturnStatement const& x) {
  if (!(*this)(*x.expr)) return false;
  builder_.CreateReturnOperation();
  return true;
}
bool IRGenerator::operator()(tcc::ast::Function const& x) { return true; }
bool IRGenerator::operator()(tcc::ast::FunctionList const& x) { return true; }

}  // namespace tcc
