#include "tcc/ir/generator.hpp"
#include "tcsl/tcsl.hpp"

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
    if (!builder_.HasVariable(x.Name))
    {
        errorHandler_(x.ID, "Undeclared variable: " + x.Name);
        return false;
    }
    auto const last = builder_.GetLastVariable(x.Name);
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
    auto argTemps = IRArgumentList {};
    for (auto const& expr : call.args)
    {
        if (!(*this)(expr))
        {
            return false;
        }
        argTemps.push_back(builder_.GetLastTemporary());
    }

    return builder_.CreateFunctionCall(call.FuncName.Name, argTemps);
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
    if (!builder_.HasVariable(x.lhs.Name))
    {
        errorHandler_(x.lhs.ID, "Undeclared variable: " + x.lhs.Name);
        return false;
    }
    auto const newKey = builder_.CreateAssignment(x.lhs.Name);
    builder_.CreateStoreOperation(newKey);
    return true;
}

bool IRGenerator::operator()(tcc::ast::VariableDeclaration const& x)
{
    if (builder_.HasVariable(x.lhs.Name))
    {
        errorHandler_(x.lhs.ID, "Duplicate variable: " + x.lhs.Name);
        return false;
    }
    bool r = (*this)(*x.rhs);
    if (r)  // don't add the variable if the RHS fails
    {
        builder_.AddVariable(x.lhs.Name);
        auto const newKey = builder_.CreateAssignment(x.lhs.Name);
        builder_.CreateStoreOperation(newKey);
    }
    return r;
}
bool IRGenerator::operator()(tcc::ast::Statement const& x) { return boost::apply_visitor(*this, x); }
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
bool IRGenerator::operator()(tcc::ast::IfStatement const& x)
{
    builder_.StartBasicBlock("if.begin");
    builder_.StartBasicBlock("if.cond");
    if (!(*this)(x.condition))
    {
        return false;
    }
    builder_.CreateIfStatementCondition();
    builder_.StartBasicBlock("if.then");
    if (!(*this)(x.then))
    {
        return false;
    }
    builder_.StartBasicBlock();

    // program_.op(IRByteCode::JumpIf, 0);              // we shall fill this
    // (0) in later std::size_t skip = program_.size() - 1;  // mark its
    // position  program_[skip] =
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
    auto args = IRArgumentList {};
    for (auto const& arg : func.args)
    {
        args.push_back(arg.Name);
    }

    if (!builder_.CreateFunction(func.FuncName.Name, args))
    {
        errorHandler_(func.FuncName.ID, "Duplicate function: " + func.FuncName.Name);
        return false;
    }

    return (*this)(func.body);
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

auto IRGenerator::Builder::HasVariable(std::string const& name) const -> bool
{
    // local var
    auto isLocal = currentFunction_->variables.find(name);
    if (isLocal != currentFunction_->variables.end())
    {
        return true;
    }

    // function argument
    auto const& args = currentFunction_->args;
    return std::any_of(std::begin(args), std::end(args),
                       [&name](auto const& arg) { return arg.first == name; });
}

auto IRGenerator::Builder::PushToStack(std::uint32_t x) -> void { stack_.emplace_back(x); }

auto IRGenerator::Builder::PopFromStack() -> IRStatement::Argument
{
    auto result = stack_.back();
    stack_.pop_back();
    return result;
}

auto IRGenerator::Builder::AddVariable(const std::string& name) -> void
{
    auto search = currentFunction_->variables.find(name);
    if (search == currentFunction_->variables.end())
    {
        currentFunction_->variables.insert({name, 0});
    }
    else
    {
        fmt::print("Tried to add {} twice to variable map\n", name);
    }
}

auto IRGenerator::Builder::GetLastVariable(std::string const& key) const -> std::string
{
    if (auto local = currentFunction_->variables.find(key);
        local != std::end(currentFunction_->variables))
    {
        auto newId = local->second - 1;
        return fmt::format("{}.{}", key, newId);
    }

    if (auto arg = currentFunction_->args.find(key); arg != std::end(currentFunction_->args))
    {
        auto newId = arg->second - 1;
        return fmt::format("{}.{}", key, newId);
    }

    TCC_ASSERT(false, "Should never get here, identifier should"
                      "either be a variable or an function argument");
    return "";
}

auto IRGenerator::Builder::CreateReturnOperation() -> void
{
    auto const first = PopFromStack();
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Return,
        .isTemporary = false,
        .destination = "",
        .first       = first,
        .second      = std::nullopt,
    });
}

auto IRGenerator::Builder::CreateBinaryOperation(IRByteCode op) -> void
{
    auto second  = PopFromStack();
    auto first   = PopFromStack();
    auto tmpName = CreateTemporaryOnStack();

    currentBlock_->statements.push_back(IRStatement {
        .type        = op,
        .isTemporary = true,
        .destination = std::move(tmpName),
        .first       = std::move(first),
        .second      = std::move(second),
    });
}

auto IRGenerator::Builder::CreateUnaryOperation(IRByteCode op) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = op,
        .isTemporary = true,
        .destination = CreateTemporaryOnStack(),
        .first       = PopFromStack(),
        .second      = {},
    });
}

auto IRGenerator::Builder::CreateStoreOperation(std::string key) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Store,
        .isTemporary = false,
        .destination = std::move(key),
        .first       = PopFromStack(),
        .second      = {},
    });
}

auto IRGenerator::Builder::CreateLoadOperation(std::string key) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Load,
        .isTemporary = true,
        .destination = CreateTemporaryOnStack(),
        .first       = key,
        .second      = {},
    });
}

auto IRGenerator::Builder::CreateArgStoreOperation(std::string key, std::string varName) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::ArgStore,
        .isTemporary = true,
        .destination = std::move(key),
        .first       = std::move(varName),
        .second      = {},
    });
}

auto IRGenerator::Builder::CreateAssignment(std::string const& key) -> std::string
{
    // local var
    auto isLocal = currentFunction_->variables.find(key);
    if (isLocal != std::end(currentFunction_->variables))
    {
        auto newId = isLocal->second++;
        return fmt::format("{}.{}", key, newId);
    }
    // func arg
    auto isArg = currentFunction_->args.find(key);
    if (isArg != std::end(currentFunction_->args))
    {
        auto newId = isArg->second++;
        return fmt::format("{}.{}", key, newId);
    }

    TCC_ASSERT(false, "Should never get here, identifier should"
                      "either be a variable or an function argument");
    return "";
}

auto IRGenerator::Builder::CreateTemporaryOnStack() -> std::string
{
    auto tmp = fmt::format("t.{}", tmpCounter_++);
    stack_.emplace_back(tmp);
    return tmp;
}

auto IRGenerator::Builder::CreateFunction(std::string name, const IRArgumentList& argsV) -> bool
{
    auto args = std::map<std::string, int> {};
    for (auto const& arg : argsV)
    {
        args.insert({arg, 0});
    }

    package_.functions.emplace_back(IRFunction {std::move(name), std::move(args), {}, {}});
    currentFunction_ = &package_.functions.back();
    StartBasicBlock("entry");
    for (auto const& arg : currentFunction_->args)
    {
        CreateArgStoreOperation(CreateAssignment(arg.first), arg.first);
    }

    return true;
}

auto IRGenerator::Builder::CreateFunctionCall(std::string name, IRArgumentList argTemps) -> bool
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Call,
        .isTemporary = {},
        .destination = CreateTemporaryOnStack(),
        .first       = std::move(name),
        .second      = argTemps,
    });
    return true;
}

void IRGenerator::Builder::CreateIfStatementCondition()
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::JumpIf,
        .isTemporary = false,
        .destination = "",
        .first       = GetLastTemporary(),
        .second      = {},
    });
}

void IRGenerator::Builder::StartBasicBlock(const std::string& suffix)
{
    auto const name = fmt::format("{}.{}", blockCounter_++, suffix);
    currentFunction_->blocks.push_back({name});
    currentBlock_ = &currentFunction_->blocks.back();
}

[[nodiscard]] auto IRGenerator::Builder::GetLastTemporary() const -> std::string
{
    return currentBlock_->statements.back().destination;
}

}  // namespace tcc
