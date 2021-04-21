#if !defined(TCC_IR_GENERATOR_HPP)
#define TCC_IR_GENERATOR_HPP

#include "tcc/ir/statement.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/error_handler.hpp"

#include <boost/function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace tcc
{

class IRGenerator
{
public:
    using result_type = bool;

    template<typename ErrorHandler>
    explicit IRGenerator(ErrorHandler& errorHandler)
    {
        using namespace boost::phoenix::arg_names;
        namespace phx = boost::phoenix;
        using boost::phoenix::function;

        errorHandler_ = function<ErrorHandler>(errorHandler)("Error! ", _2, phx::cref(errorHandler.getIterators())[_1]);
    }

    auto operator()(unsigned int x) -> bool;

    auto operator()(bool x) -> bool;

    auto operator()(tcc::ast::Nil x) -> bool;

    auto operator()(tcc::ast::Identifier const& x) -> bool;

    auto operator()(tcc::ast::Operation const& x) -> bool;

    auto operator()(tcc::ast::Unary const& x) -> bool;

    auto operator()(tcc::ast::FunctionCall const& call) -> bool;

    auto operator()(tcc::ast::Expression const& x) -> bool;

    auto operator()(tcc::ast::Assignment const& x) -> bool;

    auto operator()(tcc::ast::VariableDeclaration const& x) -> bool;

    auto operator()(tcc::ast::StatementList const& x) -> bool;

    auto operator()(tcc::ast::Statement const& x) -> bool;

    auto operator()(tcc::ast::IfStatement const& x) -> bool;

    auto operator()(tcc::ast::WhileStatement const& x) -> bool;

    auto operator()(tcc::ast::ReturnStatement const& x) -> bool;

    auto operator()(tcc::ast::Function const& func) -> bool;

    auto operator()(tcc::ast::FunctionList const& funcList) -> bool;

    auto currentPackage() -> IRPackage& { return builder_.currentPackage(); }

private:
    struct Builder
    {
        Builder() = default;

        [[nodiscard]] auto currentPackage() -> IRPackage& { return package_; }

        [[nodiscard]] auto hasVariable(std::string const& name) const -> bool;

        auto pushToStack(std::uint32_t x) -> void;

        [[nodiscard]] auto popFromStack() -> IRStatement::Argument;

        auto addVariable(const std::string& name) -> void;

        [[nodiscard]] auto getLastVariable(std::string const& key) const -> std::string;

        auto createReturnOperation() -> void;

        auto createBinaryOperation(IRByteCode op) -> void;

        auto createUnaryOperation(IRByteCode op) -> void;

        auto createStoreOperation(std::string key) -> void;

        auto createLoadOperation(std::string key) -> void;

        auto createArgStoreOperation(std::string key, std::string varName) -> void;

        [[nodiscard]] auto createAssignment(std::string const& key) -> std::string;

        [[nodiscard]] auto createTemporaryOnStack() -> std::string;

        [[nodiscard]] auto createFunction(std::string name, const IRArgumentList& argsV) -> bool;

        [[nodiscard]] auto createFunctionCall(std::string name, IRArgumentList argTemps) -> bool;

        auto createIfStatementCondition() -> void;

        auto startBasicBlock(const std::string& suffix = "") -> void;

        [[nodiscard]] auto getLastTemporary() const -> std::string;

    private:
        int tmpCounter_   = 0;
        int blockCounter_ = 0;
        std::vector<IRStatement::Argument> stack_;
        IRPackage package_ {"program"};
        IRFunction* currentFunction_ = nullptr;
        IRBasicBlock* currentBlock_  = nullptr;
    };

    Builder builder_ {};
    boost::function<void(int tag, std::string const& what)> errorHandler_;
};
}  // namespace tcc

#endif
