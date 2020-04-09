#include "tcc/ir/generator.hpp"
#include "tsl/tsl.hpp"

#include <boost/variant/apply_visitor.hpp>

namespace tcc
{

bool IRGenerator::operator()(tcc::ast::Nil /*unused*/)
{
    TCC_ASSERT(false, "");
    return false;
}

bool IRGenerator::operator()(unsigned int x)
{
    builder_.PushToStack(x);
    return true;
}
bool IRGenerator::operator()(bool /*unused*/) { return true; }

bool IRGenerator::operator()(tcc::ast::Identifier const& x)
{
    if (!builder_.HasVariable(x.name))
    {
        errorHandler_(x.id, "Undeclared variable: " + x.name);
        return false;
    }
    auto const last = builder_.GetLastVariable(x.name);
    builder_.CreateLoadOperation(last);
    return true;
}

bool IRGenerator::operator()(tcc::ast::Operation const& x)
{
    if (!boost::apply_visitor(*this, x.operand))
    {
        return false;
    }
    switch (x.operator_)
    {
        case tcc::ast::OpToken::Plus:
        {
            builder_.CreateBinaryOperation(IRByteCode::Addition);
            break;
        }
        case tcc::ast::OpToken::Minus:
        {
            builder_.CreateBinaryOperation(IRByteCode::Subtraction);
            break;
        }
        case tcc::ast::OpToken::Times:
        {
            builder_.CreateBinaryOperation(IRByteCode::Multiplication);
            break;
        }
        case tcc::ast::OpToken::Divide:
        {
            builder_.CreateBinaryOperation(IRByteCode::Division);
            break;
        }

        case tcc::ast::OpToken::Equal:
        {
            builder_.CreateBinaryOperation(IRByteCode::Equal);
            break;
        }
        case tcc::ast::OpToken::NotEqual:
        {
            builder_.CreateBinaryOperation(IRByteCode::NotEqual);
            break;
        }
        case tcc::ast::OpToken::Less:
        {
            builder_.CreateBinaryOperation(IRByteCode::Less);
            break;
        }
        case tcc::ast::OpToken::LessEqual:
        {
            builder_.CreateBinaryOperation(IRByteCode::LessEqual);
            break;
        }
        case tcc::ast::OpToken::Greater:
        {
            builder_.CreateBinaryOperation(IRByteCode::Greater);
            break;
        }
        case tcc::ast::OpToken::GreaterEqual:
        {
            builder_.CreateBinaryOperation(IRByteCode::GreaterEqual);
            break;
        }

        case tcc::ast::OpToken::And:
        {
            builder_.CreateBinaryOperation(IRByteCode::And);
            break;
        }
        case tcc::ast::OpToken::Or:
        {
            builder_.CreateBinaryOperation(IRByteCode::Or);
            break;
        }
        default: return false;
    }
    return true;
}
bool IRGenerator::operator()(tcc::ast::Unary const& x)
{
    if (!boost::apply_visitor(*this, x.operand))
    {
        return false;
    }
    switch (x.operator_)
    {
        case tcc::ast::OpToken::Positive: break;

        case tcc::ast::OpToken::Negative:
        {
            builder_.CreateUnaryOperation(IRByteCode::Negate);
            break;
        }
        case tcc::ast::OpToken::Not:
        {
            builder_.CreateUnaryOperation(IRByteCode::Not);
            break;
        }

        default: return false;
    }
    return true;
}
bool IRGenerator::operator()(tcc::ast::FunctionCall const& call)
{
    auto argTemps = std::vector<std::string> {};
    for (auto const& expr : call.args)
    {
        if (!(*this)(expr)) return false;
        argTemps.push_back(builder_.GetLastTemporary());
    }

    if (!builder_.CreateFunctionCall(call.function_name.name, argTemps))
    {
        return false;
    }

    return true;
}
bool IRGenerator::operator()(tcc::ast::Expression const& x)
{
    if (!boost::apply_visitor(*this, x.first))
    {
        return false;
    }
    for (tcc::ast::Operation const& oper : x.rest)
    {
        if (!(*this)(oper))
        {
            return false;
        }
    }
    return true;
}
bool IRGenerator::operator()(tcc::ast::Assignment const& x)
{
    if (!(*this)(x.rhs))
    {
        return false;
    }
    if (!builder_.HasVariable(x.lhs.name))
    {
        errorHandler_(x.lhs.id, "Undeclared variable: " + x.lhs.name);
        return false;
    }
    auto const newKey = builder_.CreateAssignment(x.lhs.name);
    builder_.CreateStoreOperation(newKey);
    return true;
}

bool IRGenerator::operator()(tcc::ast::VariableDeclaration const& x)
{
    if (builder_.HasVariable(x.lhs.name))
    {
        errorHandler_(x.lhs.id, "Duplicate variable: " + x.lhs.name);
        return false;
    }
    bool r = (*this)(*x.rhs);
    if (r)  // don't add the variable if the RHS fails
    {
        builder_.AddVariable(x.lhs.name);
        auto const newKey = builder_.CreateAssignment(x.lhs.name);
        builder_.CreateStoreOperation(newKey);
    }
    return r;
}
bool IRGenerator::operator()(tcc::ast::Statement const& x)
{
    return boost::apply_visitor(*this, x);
}
bool IRGenerator::operator()(tcc::ast::StatementList const& x)
{
    for (auto const& s : x)
    {
        if (!(*this)(s))
        {
            return false;
        }
    }
    return true;
}
bool IRGenerator::operator()(tcc::ast::IfStatement const& /*unused*/)
{
    // if (!(*this)(x.condition)) return false;
    // program_.op(IRByteCode::JumpIf, 0);              // we shall fill this
    // (0) in later std::size_t skip = program_.size() - 1;  // mark its
    // position if (!(*this)(x.then)) return false; program_[skip] =
    // int(program_.size() - skip);  // now we know where to jump to (after the
    // if branch)

    // if (x.else_)  // We got an alse
    // {
    //   program_[skip] += 2;                     // adjust for the "else" jump
    //   program_.op(IRByteCode::Jump, 0);                 // we shall fill this
    //   (0) in later std::size_t exit = program_.size() - 1;  // mark its
    //   position if (!(*this)(*x.else_)) return false; program_[exit] =
    //   int(program_.size() - exit);  // now we know where to jump to (after
    //   the else branch)
    // }

    return true;
}
bool IRGenerator::operator()(tcc::ast::WhileStatement const& /*unused*/)
{
    // std::size_t loop = program_.size();  // mark our position
    // if (!(*this)(x.condition)) return false;
    // program_.op(IRByteCode::JumpIf, 0);              // we shall fill this
    // (0) in later std::size_t exit = program_.size() - 1;  // mark its
    // position if (!(*this)(x.body)) return false;
    // program_.op(IRByteCode::Jump,
    //             int(loop - 1) - int(program_.size()));  // loop back
    // program_[exit] = int(program_.size() - exit);       // now we know where
    // to jump to (to exit the loop)
    return true;
}
bool IRGenerator::operator()(tcc::ast::ReturnStatement const& x)
{
    if (!(*this)(*x.expr))
    {
        return false;
    }
    builder_.CreateReturnOperation();
    return true;
}

bool IRGenerator::operator()(tcc::ast::Function const& func)
{
    auto args = std::vector<std::string> {};
    for (auto const& arg : func.args)
    {
        args.push_back(arg.name);
    }

    if (!builder_.CreateFunction(func.function_name.name, args))
    {
        errorHandler_(func.function_name.id,
                      "Duplicate function: " + func.function_name.name);
        return false;
    }

    if (!(*this)(func.body))
    {
        return false;
    }

    return true;
}

bool IRGenerator::operator()(tcc::ast::FunctionList const& funcList)
{
    for (auto const& func : funcList)
    {
        if (!(*this)(func))
        {
            return false;
        }
    }

    return true;
}

}  // namespace tcc
