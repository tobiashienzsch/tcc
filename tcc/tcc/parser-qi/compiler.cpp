#include "tcc/parser-qi/compiler.hpp"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <set>

#include "tcc/parser-qi/vm.hpp"

namespace client {
namespace code_gen {
void function::op(int a) {
  code.push_back(a);
  size_ += 1;
}

void function::op(int a, int b) {
  code.push_back(a);
  code.push_back(b);
  size_ += 2;
}

void function::op(int a, int b, int c) {
  code.push_back(a);
  code.push_back(b);
  code.push_back(c);
  size_ += 3;
}

int const* function::find_var(std::string const& name) const {
  auto const i = variables.find(name);
  if (i == variables.end()) return nullptr;
  return &i->second;
}

void function::add_var(std::string const& name) {
  std::size_t n = variables.size();
  variables[name] = n;
}

void function::link_to(std::string const& name, std::size_t addr) { functionCalls[addr] = name; }

void function::print_assembler() const {
  std::vector<int>::const_iterator pc = code.begin() + address;

  std::vector<std::string> locals(variables.size());
  for (auto const& p : variables) {
    locals[p.second] = p.first;
    std::cout << "      local       " << p.first << ", @" << p.second << std::endl;
  }

  std::map<std::size_t, std::string> lines;
  std::set<std::size_t> jumps;

  while (pc != (code.begin() + address + size_)) {
    std::string line;
    std::size_t addr = pc - code.begin();

    switch (*pc++) {
      case op_neg:
        line += "      op_neg";
        break;

      case op_not:
        line += "      op_not";
        break;

      case op_add:
        line += "      op_add";
        break;

      case op_sub:
        line += "      op_sub";
        break;

      case op_mul:
        line += "      op_mul";
        break;

      case op_div:
        line += "      op_div";
        break;

      case op_eq:
        line += "      op_eq";
        break;

      case op_neq:
        line += "      op_neq";
        break;

      case op_lt:
        line += "      op_lt";
        break;

      case op_lte:
        line += "      op_lte";
        break;

      case op_gt:
        line += "      op_gt";
        break;

      case op_gte:
        line += "      op_gte";
        break;

      case op_and:
        line += "      op_and";
        break;

      case op_or:
        line += "      op_or";
        break;

      case op_load:
        line += "      op_load     ";
        line += locals[*pc++];
        break;

      case op_store:
        line += "      op_store    ";
        line += locals[*pc++];
        break;

      case op_int:
        line += "      op_int      ";
        line += boost::lexical_cast<std::string>(*pc++);
        break;

      case op_true:
        line += "      op_true";
        break;

      case op_false:
        line += "      op_false";
        break;

      case op_jump: {
        line += "      op_jump     ";
        std::size_t pos = (pc - code.begin()) + *pc++;
        if (pos == code.size())
          line += "end";
        else
          line += boost::lexical_cast<std::string>(pos);
        jumps.insert(pos);
      } break;

      case op_jump_if: {
        line += "      op_jump_if  ";
        std::size_t pos = (pc - code.begin()) + *pc++;
        if (pos == code.size())
          line += "end";
        else
          line += boost::lexical_cast<std::string>(pos);
        jumps.insert(pos);
      } break;

      case op_call: {
        line += "      op_call     ";
        int nargs = *pc++;
        std::size_t jump = *pc++;
        line += boost::lexical_cast<std::string>(nargs) + ", ";
        assert(functionCalls.find(jump) != functionCalls.end());
        line += functionCalls.find(jump)->second;
      } break;

      case op_stk_adj:
        line += "      op_stk_adj  ";
        line += boost::lexical_cast<std::string>(*pc++);
        break;

      case op_return:
        line += "      op_return";
        break;
    }
    lines[addr] = line;
  }

  std::cout << "start:" << std::endl;
  for (auto const& l : lines) {
    std::size_t pos = l.first;
    if (jumps.find(pos) != jumps.end()) std::cout << pos << ':' << std::endl;
    std::cout << l.second << std::endl;
  }

  std::cout << "end:" << std::endl << std::endl;
}

bool compiler::operator()(unsigned int x) {
  assert(current != nullptr);
  current->op(op_int, static_cast<int>(x));
  return true;
}

bool compiler::operator()(bool x) {
  assert(current != nullptr);
  current->op(x ? op_true : op_false);
  return true;
}

bool compiler::operator()(ast::Identifier const& x) {
  assert(current != nullptr);
  int const* p = current->find_var(x.name);
  if (p == nullptr) {
    errorHandler_(x.id, "Undeclared variable: " + x.name);
    return false;
  }
  current->op(op_load, *p);
  return true;
}

bool compiler::operator()(ast::operation const& x) {
  assert(current != nullptr);
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::OpToken::Plus:
      current->op(op_add);
      break;
    case ast::OpToken::Minus:
      current->op(op_sub);
      break;
    case ast::OpToken::Times:
      current->op(op_mul);
      break;
    case ast::OpToken::Divide:
      current->op(op_div);
      break;

    case ast::OpToken::Equal:
      current->op(op_eq);
      break;
    case ast::OpToken::NotEqual:
      current->op(op_neq);
      break;
    case ast::OpToken::Less:
      current->op(op_lt);
      break;
    case ast::OpToken::LessEqual:
      current->op(op_lte);
      break;
    case ast::OpToken::Greater:
      current->op(op_gt);
      break;
    case ast::OpToken::GreaterEqual:
      current->op(op_gte);
      break;

    case ast::OpToken::And:
      current->op(op_and);
      break;
    case ast::OpToken::Or:
      current->op(op_or);
      break;
    default:
      assert(false);
      return false;
  }
  return true;
}

bool compiler::operator()(ast::Unary const& x) {
  assert(current != nullptr);
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::OpToken::Negative:
      current->op(op_neg);
      break;
    case ast::OpToken::Not:
      current->op(op_not);
      break;
    case ast::OpToken::Positive:
      break;
    default:
      assert(false);
      return false;
  }
  return true;
}

bool compiler::operator()(ast::FunctionCall const& x) {
  assert(current != nullptr);

  if (functions.find(x.function_name.name) == functions.end()) {
    errorHandler_(x.function_name.id, "Function not found: " + x.function_name.name);
    return false;
  }

  boost::shared_ptr<code_gen::function> p = functions[x.function_name.name];

  if (p->nargs() != static_cast<int>(x.args.size())) {
    errorHandler_(x.function_name.id, "Wrong number of arguments: " + x.function_name.name);
    return false;
  }

  for (ast::Expression const& expr : x.args) {
    if (!(*this)(expr)) return false;
  }

  current->op(op_call, p->nargs(), p->get_address());
  current->link_to(x.function_name.name, p->get_address());

  return true;
}

bool compiler::operator()(ast::Expression const& x) {
  assert(current != nullptr);
  if (!boost::apply_visitor(*this, x.first)) return false;
  for (ast::operation const& oper : x.rest) {
    if (!(*this)(oper)) return false;
  }
  return true;
}

bool compiler::operator()(ast::Assignment const& x) {
  assert(current != nullptr);
  if (!(*this)(x.rhs)) return false;
  int const* p = current->find_var(x.lhs.name);
  if (p == nullptr) {
    errorHandler_(x.lhs.id, "Undeclared variable: " + x.lhs.name);
    return false;
  }
  current->op(op_store, *p);
  return true;
}

bool compiler::operator()(ast::VariableDeclaration const& x) {
  assert(current != nullptr);
  int const* p = current->find_var(x.lhs.name);
  if (p != 0) {
    errorHandler_(x.lhs.id, "Duplicate variable: " + x.lhs.name);
    return false;
  }
  if (x.rhs)  // if there's an RHS initializer
  {
    bool r = (*this)(*x.rhs);
    if (r)  // don't add the variable if the RHS fails
    {
      current->add_var(x.lhs.name);
      current->op(op_store, *current->find_var(x.lhs.name));
    }
    return r;
  } else {
    current->add_var(x.lhs.name);
  }
  return true;
}

bool compiler::operator()(ast::Statement const& x) {
  assert(current != nullptr);
  return boost::apply_visitor(*this, x);
}

bool compiler::operator()(ast::StatementList const& x) {
  assert(current != nullptr);
  for (auto const& s : x) {
    if (!(*this)(s)) return false;
  }
  return true;
}

bool compiler::operator()(ast::IfStatement const& x) {
  assert(current != nullptr);
  if (!(*this)(x.condition)) return false;
  current->op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t skip = current->size() - 1;  // mark its position
  if (!(*this)(x.then)) return false;
  // now we know where to jump to (after the if branch)
  (*current)[skip] = current->size() - static_cast<size_t>(skip);

  if (x.else_)  // We got an else
  {
    (*current)[skip] += 2;                   // adjust for the "else" jump
    current->op(op_jump, 0);                 // we shall fill this (0) in later
    std::size_t exit = current->size() - 1;  // mark its position
    if (!(*this)(*x.else_)) return false;
    // now we know where to jump to (after the else branch)
    (*current)[exit] = current->size() - static_cast<size_t>(exit);
  }

  return true;
}

bool compiler::operator()(ast::WhileStatement const& x) {
  assert(current != nullptr);
  std::size_t loop = current->size();  // mark our position
  if (!(*this)(x.condition)) return false;
  current->op(op_jump_if, 0);              // we shall fill this (0) in later
  std::size_t exit = current->size() - 1;  // mark its position
  if (!(*this)(x.body)) return false;
  current->op(op_jump,
              int(loop - 1) - int(current->size()));  // loop back

  // now we know where to jump to (to exit the loop)
  (*current)[exit] = current->size() - static_cast<size_t>(exit);
  return true;
}

bool compiler::operator()(ast::ReturnStatement const& x) {
  if (void_return) {
    if (x.expr) {
      errorHandler_(x.id, "'void' function returning a value: ");
      return false;
    }
  } else {
    if (!x.expr) {
      errorHandler_(x.id, current_function_name + " function must return a value: ");
      return false;
    }
  }

  if (x.expr) {
    if (!(*this)(*x.expr)) return false;
  }
  current->op(op_return);
  return true;
}

bool compiler::operator()(ast::function const& x) {
  void_return = x.return_type == "void";
  if (functions.find(x.function_name.name) != functions.end()) {
    errorHandler_(x.function_name.id, "Duplicate function: " + x.function_name.name);
    return false;
  }
  boost::shared_ptr<code_gen::function>& p = functions[x.function_name.name];
  p.reset(new code_gen::function(code, x.args.size()));
  current = p.get();
  current_function_name = x.function_name.name;

  // op_stk_adj 0 for now. we'll know how many variables
  // we'll have later and add them
  current->op(op_stk_adj, 0);
  for (ast::Identifier const& arg : x.args) {
    current->add_var(arg.name);
  }

  if (!(*this)(x.body)) return false;
  (*current)[1] = current->nvars();  // now store the actual number of variables
                                     // this includes the arguments
  return true;
}

bool compiler::operator()(ast::FunctionList const& x) {
  // Jump to the main function
  code.push_back(op_jump);
  code.push_back(0);  // we will fill this in later when we finish compiling
                      // and we know where the main function is

  for (ast::function const& f : x) {
    if (!(*this)(f)) {
      code.clear();
      return false;
    }
  }
  // find the main function
  boost::shared_ptr<code_gen::function> p = find_function("main");

  if (!p)  // main function not found
  {
    std::cerr << "Error: main function not defined" << std::endl;
    return false;
  }
  code[1] = p->get_address() - 1;  // jump to this (main function) address

  return true;
}

void compiler::print_assembler() const {
  for (auto const& p : functions) {
    std::cout << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
    std::cout << p.second->get_address() << ": function " << p.first << std::endl;
    p.second->print_assembler();
  }
}

boost::shared_ptr<code_gen::function> compiler::find_function(std::string const& name) const {
  if (auto const i = functions.find(name); i == functions.end()) {
    return boost::shared_ptr<code_gen::function>();
  } else {
    return i->second;
  }
}
}  // namespace code_gen
}  // namespace client
