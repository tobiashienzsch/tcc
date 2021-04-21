#include "tcc/ir/generator.hpp"
#include "tcsl/tcsl.hpp"

#include <boost/variant/apply_visitor.hpp>

namespace tcc
{

auto IRGenerator::operator()(tcc::ast::Nil /*unused*/) -> bool
{
    TCC_ASSERT(false, "");
    return false;
}

auto IRGenerator::operator()(unsigned int x) -> bool
{
    builder_.pushToStack(x);
    return true;
}
auto IRGenerator::operator()(bool /*unused*/) -> bool { return true; }

auto IRGenerator::operator()(tcc::ast::Identifier const& x) -> bool
{
    if (!builder_.hasVariable(x.name))
    {
        errorHandler_(x.id, "Undeclared variable: " + x.name);
        return false;
    }
    auto const last = builder_.getLastVariable(x.name);
    builder_.createLoadOperation(last);
    return true;
}

auto IRGenerator::operator()(tcc::ast::Operation const& x) -> bool
{
    if (!boost::apply_visitor(*this, x.operand)) { return false; }
    switch (x.Operator)
    {
        case tcc::ast::OpToken::Plus:
        {
            builder_.createBinaryOperation(IRByteCode::Addition);
            break;
        }
        case tcc::ast::OpToken::Minus:
        {
            builder_.createBinaryOperation(IRByteCode::Subtraction);
            break;
        }
        case tcc::ast::OpToken::Times:
        {
            builder_.createBinaryOperation(IRByteCode::Multiplication);
            break;
        }
        case tcc::ast::OpToken::Divide:
        {
            builder_.createBinaryOperation(IRByteCode::Division);
            break;
        }

        case tcc::ast::OpToken::Equal:
        {
            builder_.createBinaryOperation(IRByteCode::Equal);
            break;
        }
        case tcc::ast::OpToken::NotEqual:
        {
            builder_.createBinaryOperation(IRByteCode::NotEqual);
            break;
        }
        case tcc::ast::OpToken::Less:
        {
            builder_.createBinaryOperation(IRByteCode::Less);
            break;
        }
        case tcc::ast::OpToken::LessEqual:
        {
            builder_.createBinaryOperation(IRByteCode::LessEqual);
            break;
        }
        case tcc::ast::OpToken::Greater:
        {
            builder_.createBinaryOperation(IRByteCode::Greater);
            break;
        }
        case tcc::ast::OpToken::GreaterEqual:
        {
            builder_.createBinaryOperation(IRByteCode::GreaterEqual);
            break;
        }

        case tcc::ast::OpToken::And:
        {
            builder_.createBinaryOperation(IRByteCode::And);
            break;
        }
        case tcc::ast::OpToken::Or:
        {
            builder_.createBinaryOperation(IRByteCode::Or);
            break;
        }
        default: return false;
    }
    return true;
}
auto IRGenerator::operator()(tcc::ast::Unary const& x) -> bool
{
    if (!boost::apply_visitor(*this, x.operand)) { return false; }
    switch (x.Operator)
    {
        case tcc::ast::OpToken::Positive: break;

        case tcc::ast::OpToken::Negative:
        {
            builder_.createUnaryOperation(IRByteCode::Negate);
            break;
        }
        case tcc::ast::OpToken::Not:
        {
            builder_.createUnaryOperation(IRByteCode::Not);
            break;
        }

        default: return false;
    }
    return true;
}
auto IRGenerator::operator()(tcc::ast::FunctionCall const& call) -> bool
{
    auto argTemps = IRArgumentList {};
    for (auto const& expr : call.args)
    {
        if (!(*this)(expr)) { return false; }
        argTemps.pushBack(builder_.getLastTemporary());
    }

    return builder_.createFunctionCall(call.funcName.name, argTemps);
}
auto IRGenerator::operator()(tcc::ast::Expression const& x) -> bool
{
    if (!boost::apply_visitor(*this, x.first)) { return false; }
    for (tcc::ast::Operation const& oper : x.rest)
    {
        if (!(*this)(oper)) { return false; }
    }
    return true;
}
auto IRGenerator::operator()(tcc::ast::Assignment const& x) -> bool
{
    if (!(*this)(x.right)) { return false; }
    if (!builder_.hasVariable(x.left.name))
    {
        errorHandler_(x.left.id, "Undeclared variable: " + x.left.name);
        return false;
    }
    auto const newKey = builder_.createAssignment(x.left.name);
    builder_.createStoreOperation(newKey);
    return true;
}

auto IRGenerator::operator()(tcc::ast::VariableDeclaration const& x) -> bool
{
    if (builder_.hasVariable(x.left.name))
    {
        errorHandler_(x.left.id, "Duplicate variable: " + x.left.name);
        return false;
    }
    bool r = (*this)(*x.right);
    if (r)  // don't add the variable if the RHS fails
    {
        builder_.addVariable(x.left.name);
        auto const newKey = builder_.createAssignment(x.left.name);
        builder_.createStoreOperation(newKey);
    }
    return r;
}
auto IRGenerator::operator()(tcc::ast::Statement const& x) -> bool { return boost::apply_visitor(*this, x); }
auto IRGenerator::operator()(tcc::ast::StatementList const& x) -> bool
{
    for (auto const& s : x)
    {
        if (!(*this)(s)) { return false; }
    }
    return true;
}
auto IRGenerator::operator()(tcc::ast::IfStatement const& x) -> bool
{
    builder_.startBasicBlock("if.begin");
    builder_.startBasicBlock("if.cond");
    if (!(*this)(x.condition)) { return false; }
    builder_.createIfStatementCondition();
    builder_.startBasicBlock("if.then");
    if (!(*this)(x.then)) { return false; }
    builder_.startBasicBlock();

    // program_.op(IRByteCode::JumpIf, 0);              // we shall fill this
    // (0) in later std::size_t skip = program_.size() - 1;  // mark its
    // position  program_[skip] =
    // int(program_.size() - skip);  // now we know where to jump to (after the
    // if branch)

    // if (x.Else)  // We got an alse
    // {
    //   program_[skip] += 2;                     // adjust for the "else" jump
    //   program_.op(IRByteCode::Jump, 0);                 // we shall fill this
    //   (0) in later std::size_t exit = program_.size() - 1;  // mark its
    //   position if (!(*this)(*x.Else)) return false; program_[exit] =
    //   int(program_.size() - exit);  // now we know where to jump to (after
    //   the else branch)
    // }

    return true;
}
auto IRGenerator::operator()(tcc::ast::WhileStatement const& /*unused*/) -> bool
{
    // std::size_t loop = program_.size();  // mark our position
    // if (!(*this)(x.Condition)) return false;
    // program_.op(IRByteCode::JumpIf, 0);              // we shall fill this
    // (0) in later std::size_t exit = program_.size() - 1;  // mark its
    // position if (!(*this)(x.Body)) return false;
    // program_.op(IRByteCode::Jump,
    //             int(loop - 1) - int(program_.size()));  // loop back
    // program_[exit] = int(program_.size() - exit);       // now we know where
    // to jump to (to exit the loop)
    return true;
}
auto IRGenerator::operator()(tcc::ast::ReturnStatement const& x) -> bool
{
    if (!(*this)(*x.expr)) { return false; }
    builder_.createReturnOperation();
    return true;
}

auto IRGenerator::operator()(tcc::ast::Function const& func) -> bool
{
    auto args = IRArgumentList {};
    for (auto const& arg : func.args) { args.pushBack(arg.name); }

    if (!builder_.createFunction(func.funcName.name, args))
    {
        errorHandler_(func.funcName.id, "Duplicate function: " + func.funcName.name);
        return false;
    }

    return (*this)(func.body);
}

auto IRGenerator::operator()(tcc::ast::FunctionList const& funcList) -> bool
{
    for (auto const& func : funcList)
    {
        if (!(*this)(func)) { return false; }
    }

    return true;
}

auto IRGenerator::Builder::hasVariable(std::string const& name) const -> bool
{
    // local var
    auto isLocal = currentFunction_->variables.find(name);
    if (isLocal != currentFunction_->variables.end()) { return true; }

    // function argument
    auto const& args = currentFunction_->args;
    return std::any_of(std::begin(args), std::end(args), [&name](auto const& arg) { return arg.first == name; });
}

auto IRGenerator::Builder::pushToStack(std::uint32_t x) -> void { stack_.emplace_back(x); }

auto IRGenerator::Builder::popFromStack() -> IRStatement::Argument
{
    auto result = stack_.back();
    stack_.pop_back();
    return result;
}

auto IRGenerator::Builder::addVariable(const std::string& name) -> void
{
    auto search = currentFunction_->variables.find(name);
    if (search == currentFunction_->variables.end()) { currentFunction_->variables.insert({name, 0}); }
    else
    {
        fmt::print("Tried to add {} twice to variable map\n", name);
    }
}

auto IRGenerator::Builder::getLastVariable(std::string const& key) const -> std::string
{
    if (auto local = currentFunction_->variables.find(key); local != std::end(currentFunction_->variables))
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

auto IRGenerator::Builder::createReturnOperation() -> void
{
    auto const first = popFromStack();
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Return,
        .isTemporary = false,
        .destination = "",
        .first       = first,
        .second      = std::nullopt,
    });
}

auto IRGenerator::Builder::createBinaryOperation(IRByteCode op) -> void
{
    auto second  = popFromStack();
    auto first   = popFromStack();
    auto tmpName = createTemporaryOnStack();

    currentBlock_->statements.push_back(IRStatement {
        .type        = op,
        .isTemporary = true,
        .destination = std::move(tmpName),
        .first       = std::move(first),
        .second      = std::move(second),
    });
}

auto IRGenerator::Builder::createUnaryOperation(IRByteCode op) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = op,
        .isTemporary = true,
        .destination = createTemporaryOnStack(),
        .first       = popFromStack(),
        .second      = {},
    });
}

auto IRGenerator::Builder::createStoreOperation(std::string key) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Store,
        .isTemporary = false,
        .destination = std::move(key),
        .first       = popFromStack(),
        .second      = {},
    });
}

auto IRGenerator::Builder::createLoadOperation(std::string key) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Load,
        .isTemporary = true,
        .destination = createTemporaryOnStack(),
        .first       = key,
        .second      = {},
    });
}

auto IRGenerator::Builder::createArgStoreOperation(std::string key, std::string varName) -> void
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::ArgStore,
        .isTemporary = true,
        .destination = std::move(key),
        .first       = std::move(varName),
        .second      = {},
    });
}

auto IRGenerator::Builder::createAssignment(std::string const& key) -> std::string
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

auto IRGenerator::Builder::createTemporaryOnStack() -> std::string
{
    auto tmp = fmt::format("t.{}", tmpCounter_++);
    stack_.emplace_back(tmp);
    return tmp;
}

auto IRGenerator::Builder::createFunction(std::string name, const IRArgumentList& argsV) -> bool
{
    auto args = std::map<std::string, int> {};
    for (auto const& arg : argsV) { args.insert({arg, 0}); }

    package_.functions.emplace_back(IRFunction {std::move(name), std::move(args), {}, {}});
    currentFunction_ = &package_.functions.back();
    startBasicBlock("entry");
    for (auto const& arg : currentFunction_->args) { createArgStoreOperation(createAssignment(arg.first), arg.first); }

    return true;
}

auto IRGenerator::Builder::createFunctionCall(std::string name, IRArgumentList argTemps) -> bool
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::Call,
        .isTemporary = {},
        .destination = createTemporaryOnStack(),
        .first       = std::move(name),
        .second      = argTemps,
    });
    return true;
}

void IRGenerator::Builder::createIfStatementCondition()
{
    currentBlock_->statements.push_back(IRStatement {
        .type        = IRByteCode::JumpIf,
        .isTemporary = false,
        .destination = "",
        .first       = getLastTemporary(),
        .second      = {},
    });
}

void IRGenerator::Builder::startBasicBlock(const std::string& suffix)
{
    auto const name = fmt::format("{}.{}", blockCounter_++, suffix);
    currentFunction_->blocks.push_back({name});
    currentBlock_ = &currentFunction_->blocks.back();
}

[[nodiscard]] auto IRGenerator::Builder::getLastTemporary() const -> std::string
{
    return currentBlock_->statements.back().destination;
}

}  // namespace tcc
