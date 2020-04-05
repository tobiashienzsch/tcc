#include "tcc/parser/ast_printer.hpp"

#include "tsl/tsl.hpp"

namespace tcc::parser {

auto AstPrinter::operator()(uint64_t x) -> bool {
  pushToStack(x);
  return true;
}

auto AstPrinter::operator()(bool x) -> bool {
  //
  return true;
}

auto AstPrinter::operator()(ast::Variable const& x) -> bool {
  // auto const last = m_builder.GetLastVariable(x.name);
  // createLoadOperation(last);
  return true;
}

auto AstPrinter::operator()(ast::Operation const& x) -> bool {
  printIndentation();
  fmt::print("<BinaryOperation>\n");
  _identationLevel++;
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::OpToken::Plus: {
      createBinaryOperation(op_add);
      break;
    }
    case ast::OpToken::Minus: {
      createBinaryOperation(op_sub);
      break;
    }
    case ast::OpToken::Times: {
      createBinaryOperation(op_mul);
      break;
    }
    case ast::OpToken::Divide: {
      createBinaryOperation(op_div);
      break;
    }

    case ast::OpToken::Equal: {
      createBinaryOperation(op_eq);
      break;
    }
    case ast::OpToken::NotEqual: {
      createBinaryOperation(op_neq);
      break;
    }
    case ast::OpToken::Less: {
      createBinaryOperation(op_lt);
      break;
    }
    case ast::OpToken::LessEqual: {
      createBinaryOperation(op_lte);
      break;
    }
    case ast::OpToken::Greater: {
      createBinaryOperation(op_gt);
      break;
    }
    case ast::OpToken::GreaterEqual: {
      createBinaryOperation(op_gte);
      break;
    }

    case ast::OpToken::And: {
      createBinaryOperation(op_and);
      break;
    }
    case ast::OpToken::Or: {
      createBinaryOperation(op_or);
      break;
    }
    default:
      return false;
  }

  _identationLevel--;
  printIndentation();
  fmt::print("<BinaryOperation>\n");
  return true;
}

auto AstPrinter::operator()(ast::Unary const& x) -> bool {
  if (!boost::apply_visitor(*this, x.operand_)) return false;
  switch (x.operator_) {
    case ast::OpToken::Positive:
      break;

    case ast::OpToken::Negative: {
      createUnaryOperation(op_neg);
      break;
    }
    case ast::OpToken::Not: {
      createUnaryOperation(op_not);
      break;
    }

    default:
      return false;
  }
  return true;
}

auto AstPrinter::operator()(ast::Expression const& x) -> bool {
  if (!boost::apply_visitor(*this, x.first)) return false;
  for (ast::Operation const& oper : x.rest) {
    if (!(*this)(oper)) return false;
  }
  return true;
}

auto AstPrinter::operator()(ast::Assignment const& x) -> bool {
  if (!(*this)(x.rhs)) return false;
  fmt::print("<Assignment name={}>\n", x.lhs.name);
  return true;
}

auto AstPrinter::operator()(ast::VariableDeclaration const& x) -> bool {
  printIndentation();
  fmt::print("<Variable Declaration name={}>\n", x.assign.lhs.name);
  _identationLevel++;
  auto const r = (*this)(x.assign.rhs);
  if (r) {
  }
  _identationLevel--;
  printIndentation();
  fmt::print("</Variable Declaration name={}>\n", x.assign.lhs.name);
  return r;
}

auto AstPrinter::operator()(ast::Statement const& x) -> bool { return boost::apply_visitor(*this, x); }

auto AstPrinter::operator()(ast::StatementList const& x) -> bool {
  printIndentation();
  fmt::print("<Statement List>\n");
  _identationLevel++;
  for (auto const& s : x) {
    if (!(*this)(s)) return false;
  }
  _identationLevel--;
  printIndentation();
  fmt::print("</Statement List>\n");
  return true;
}

auto AstPrinter::operator()(ast::IfStatement const& x) -> bool {
  // if (!(*this)(x.condition)) return false;
  //
  // std::size_t skip = program.size() - 1;  // mark its position
  // if (!(*this)(x.then)) return false;
  // program[skip] = int(program.size() - skip);  // now we know where to jump to (after the if branch)

  // if (x.else_)  // We got an alse
  // {
  //   program[skip] += 2;                     // adjust for the "else" jump
  //
  //   std::size_t exit = program.size() - 1;  // mark its position
  //   if (!(*this)(*x.else_)) return false;
  //   program[exit] = int(program.size() - exit);  // now we know where to jump to (after the else branch)
  // }

  return true;
}

auto AstPrinter::operator()(ast::WhileStatement const& x) -> bool {
  // std::size_t loop = program.size();  // mark our position
  // if (!(*this)(x.condition)) return false;
  //
  // std::size_t exit = program.size() - 1;  // mark its position
  // if (!(*this)(x.body)) return false;
  //
  //            int(loop - 1) - int(program.size()));  // loop back
  // program[exit] = int(program.size() - exit);       // now we know where to jump to (to exit the loop)
  return true;
}

bool AstPrinter::operator()(ast::ReturnStatement const& x) {
  if (!(*this)(x.expression)) return false;
  printReturnOperation();
  return true;
}

auto AstPrinter::start(ast::StatementList const& x) -> bool {
  // program.clear();
  // op_stk_adj 0 for now. we'll know how many variables we'll have later
  //

  if (!(*this)(x)) {
    // program.clear();
    return false;
  }
  // program[1] = int(program.nvars());  // now store the actual number of variables
  return true;
}
}  // namespace tcc::parser