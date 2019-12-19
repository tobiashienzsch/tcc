#pragma once

#include "ast.hpp"
#include "error_handler.hpp"
#include "vm.hpp"

#include "fmt/format.h"

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace client
{
namespace code_gen
{

struct ir_builder
{
    struct temp
    {
        std::string name;
    };

    void PushToStack(int x) { m_stack.push_back(x); }

    void CreateBinaryOperation(int op)
    {
        auto second = m_stack.back();
        m_stack.pop_back();

        auto secondStr = std::string {};
        if (std::holds_alternative<int>(second)) secondStr = std::to_string(std::get<int>(second));
        if (std::holds_alternative<temp>(second)) secondStr = fmt::format("%{}", std::get<temp>(second).name);

        auto first = m_stack.back();
        m_stack.pop_back();

        auto firstStr = std::string {};
        if (std::holds_alternative<int>(first)) firstStr = std::to_string(std::get<int>(first));
        if (std::holds_alternative<temp>(first)) firstStr = fmt::format("%{}", std::get<temp>(first).name);

        auto tmpName = std::string {"t"}.append(std::to_string(m_varCounter++));
        auto tmp     = temp {tmpName};
        m_stack.push_back(tmp);

        std::stringstream opCodeStr;
        opCodeStr << static_cast<byte_code>(op);
        fmt::print("{0}\t:=\t{1}\t{2}\t{3}\n", tmp.name, firstStr, opCodeStr.str(), secondStr);
    }

    void CreateUnaryOperation(int op)
    {
        auto first = m_stack.back();
        m_stack.pop_back();

        auto firstStr = std::string {};
        if (std::holds_alternative<int>(first)) firstStr = std::to_string(std::get<int>(first));
        if (std::holds_alternative<temp>(first)) firstStr = fmt::format("%{}", std::get<temp>(first).name);

        auto tmpName = std::string {"t"}.append(std::to_string(m_varCounter++));
        auto tmp     = temp {tmpName};
        m_stack.push_back(tmp);

        auto opCodeStr = std::stringstream {};
        opCodeStr << static_cast<byte_code>(op);
        fmt::print("{0}\t:=\t{1}\t{2}\n", tmp.name, firstStr, opCodeStr.str());
    }

    void CreateStoreOperation(std::string name)
    {
        auto first = m_stack.back();
        m_stack.pop_back();
        auto firstStr = std::string {};
        if (std::holds_alternative<int>(first)) firstStr = std::to_string(std::get<int>(first));
        if (std::holds_alternative<temp>(first)) firstStr = fmt::format("%{}", std::get<temp>(first).name);

        fmt::print("{0}\t:=\t{1}\tstore\n", name, firstStr);
    }

    void CreateLoadOperation(std::string name)
    {
        auto tmpName = std::string {"t"}.append(std::to_string(m_varCounter++));
        auto tmp     = temp {tmpName};
        m_stack.push_back(tmp);

        fmt::print("{0}\t:=\t%{1}\tload\n", tmpName, name);
    }

private:
    int m_varCounter = 0;
    std::map<std::string, int> variables;
    std::vector<std::variant<int, temp>> m_stack;
};

///////////////////////////////////////////////////////////////////////////
//  The Program
///////////////////////////////////////////////////////////////////////////
struct program
{
    void op(int a);
    void op(int a, int b);
    // void op(int a, int b, int c);

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
    typedef bool result_type;
    typedef std::function<void(x3::position_tagged, std::string const&)> error_handler_type;

    template<typename ErrorHandler>
    compiler(client::code_gen::program& program, ir_builder& builder, ErrorHandler const& error_handler)
        : program(program)
        , m_builder(builder)
        , error_handler([&](x3::position_tagged pos, std::string const& msg) { error_handler(pos, msg); })
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

    client::code_gen::program& program;
    ir_builder& m_builder;
    error_handler_type error_handler;
};
}  // namespace code_gen
}  // namespace client
