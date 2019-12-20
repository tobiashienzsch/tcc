#pragma once

#include "ast.hpp"
#include "error_handler.hpp"
#include "ir/three_address_code.hpp"
#include "vm.hpp"

#include "tcc/core/core.hpp"

#include "fmt/format.h"

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace tcc
{
namespace code_gen
{

struct IntermediateRepresentation
{

    auto PushToStack(int x) -> void { m_stack.push_back(x); }

    auto PopFromStack() -> std::variant<int, std::string>
    {
        auto const result = m_stack.back();
        m_stack.pop_back();
        return result;
    }

    auto AddVariable(std::string name) -> void
    {
        auto search = m_variables.find(name);
        if (search == m_variables.end())
            m_variables.insert({name, 0});
        else
            fmt::print("Tried to add {} twice to variable map\n", name);
    }

    auto CreateBinaryOperation(byte_code op) -> void
    {
        auto const second  = PopFromStack();
        auto const first   = PopFromStack();
        auto const tmpName = CreateTemporaryOnStack();

        m_statements.push_back(ThreeAddressCode {op, tmpName, first, second});
    }

    auto CreateUnaryOperation(byte_code op) -> void
    {
        auto const first   = PopFromStack();
        auto const tmpName = CreateTemporaryOnStack();
        m_statements.push_back(ThreeAddressCode {op, tmpName, first, {}});
    }

    auto CreateStoreOperation(std::string key) -> void
    {
        auto const first = PopFromStack();
        m_statements.push_back(ThreeAddressCode {op_store, key, first, {}});
    }

    auto CreateLoadOperation(std::string key) -> void
    {
        auto const tmpName = CreateTemporaryOnStack();
        m_statements.push_back(ThreeAddressCode {op_load, tmpName, key, {}});
    }

    auto CreateAssignment(std::string const& key) -> std::string
    {
        auto search = m_variables.find(key);
        auto newId  = search->second++;
        return fmt::format("{}{}", key, newId);
    }

    auto GetLastVariable(std::string const& key) -> std::string
    {
        auto search = m_variables.find(key);
        auto newId  = search->second - 1;
        return fmt::format("{}{}", key, newId);
    }

    void PrintStatementList() const
    {
        fmt::print("#locals: {}\n", m_variables.size());

        for (auto const& x : m_statements)
        {
            std::cout << x << '\n';
        }

        fmt::print("-------\n\n", m_variables.size());
    }

    auto CreateTemporaryOnStack() -> std::string
    {
        auto tmp = std::string("t").append(std::to_string(m_varCounter++));
        m_stack.push_back(tmp);
        return tmp;
    }

private:
    int m_varCounter = 0;
    std::map<std::string, int> m_variables;
    std::vector<std::variant<int, std::string>> m_stack;
    std::vector<ThreeAddressCode> m_statements;
};

struct program
{
    void op(int a);
    void op(int a, int b);

    int& operator[](std::size_t i) { return code[i]; }
    int operator[](std::size_t i) const { return code[i]; }
    void clear()
    {
        code.clear();
        variables.clear();
    }
    std::size_t size() const { return code.size(); }
    std::vector<int> const& operator()() const { return code; }

    std::size_t nvars() const { return variables.size(); }
    int const* find_var(std::string const& name) const;
    void add_var(std::string const& name);

    void print_variables(std::vector<int> const& stack) const;
    void print_assembler() const;

private:
    std::map<std::string, int> variables;
    std::vector<int> code;
};

////////////////////////////////////////////////////////////////////////////
//  The Compiler
////////////////////////////////////////////////////////////////////////////
struct compiler
{
    using result_type        = bool;
    using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

    template<typename ErrorHandler>
    compiler(tcc::code_gen::program& prog, IntermediateRepresentation& builder, ErrorHandler const& errorHandler)
        : program(prog)
        , m_builder(builder)
        , error_handler([&](x3::position_tagged pos, std::string const& msg) { errorHandler(pos, msg); })
    {
    }

    bool operator()(ast::nil) const
    {
        BOOST_ASSERT(0);
        return false;
    }
    bool operator()(unsigned int x) const;
    bool operator()(bool x) const;
    bool operator()(ast::variable const& x) const;
    bool operator()(ast::operation const& x) const;
    bool operator()(ast::unary const& x) const;
    bool operator()(ast::expression const& x) const;
    bool operator()(ast::assignment const& x) const;
    bool operator()(ast::variable_declaration const& x) const;
    bool operator()(ast::statement_list const& x) const;
    bool operator()(ast::statement const& x) const;
    bool operator()(ast::if_statement const& x) const;
    bool operator()(ast::while_statement const& x) const;

    bool start(ast::statement_list const& x) const;

    tcc::code_gen::program& program;
    IntermediateRepresentation& m_builder;
    error_handler_type error_handler;
};
}  // namespace code_gen
}  // namespace tcc
