#pragma once

#include "ast.hpp"
#include "error_handler.hpp"
#include "vm.hpp"

#include "fmt/format.h"

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace client
{
namespace code_gen
{

struct ir_builder
{
    struct ir_temp
    {
        std::string name;
    };

    struct ir_statement
    {
        byte_code type;
        std::string destination;
        std::variant<int, ir_temp> first;
        std::optional<std::variant<int, ir_temp>> second;

        friend std::ostream& operator<<(std::ostream& out, ir_statement const& data)
        {
            auto firstStr = std::string {};
            if (std::holds_alternative<int>(data.first)) firstStr = std::to_string(std::get<int>(data.first));
            if (std::holds_alternative<ir_temp>(data.first))
                firstStr = fmt::format("%{}", std::get<ir_temp>(data.first).name);

            auto secondStr = std::string {};
            if (data.second.has_value())
            {
                auto const second = data.second.value();
                if (std::holds_alternative<int>(second)) secondStr = std::to_string(std::get<int>(second));
                if (std::holds_alternative<ir_temp>(second))
                    secondStr = fmt::format("%{}", std::get<ir_temp>(second).name);
            }

            std::stringstream opCodeStr;
            opCodeStr << static_cast<byte_code>(data.type);
            auto const str
                = fmt::format("{0}\t:=\t{1}\t{2}\t{3}", data.destination, firstStr, opCodeStr.str(), secondStr);

            return out << str;
        }
    };

    void PushToStack(int x) { m_stack.push_back(x); }

    auto PopFromStack()
    {
        auto result = m_stack.back();
        m_stack.pop_back();
        return result;
    }

    void CreateBinaryOperation(int op)
    {
        auto const second  = PopFromStack();
        auto const first   = PopFromStack();
        auto const tmpName = CreateTemporaryOnStack();

        m_statements.push_back(ir_statement {static_cast<byte_code>(op), tmpName, first, second});
    }

    void CreateUnaryOperation(int op)
    {
        auto const first   = PopFromStack();
        auto const tmpName = CreateTemporaryOnStack();
        m_statements.push_back(ir_statement {static_cast<byte_code>(op), tmpName, first});
    }

    void CreateStoreOperation(std::string name)
    {
        auto first = PopFromStack();

        m_statements.push_back(ir_statement {op_store, name, first});
    }

    void CreateLoadOperation(std::string name)
    {
        auto const tmpName = CreateTemporaryOnStack();
        m_statements.push_back(ir_statement {op_load, tmpName, ir_temp {name}});
    }

    void PrintStatementList() const
    {
        for (auto const& x : m_statements)
        {
            std::cout << x << '\n';
        }
        std::cout << "\n\n";
    }

    auto CreateTemporaryOnStack() -> std::string
    {

        auto tmpName = std::string {"t"}.append(std::to_string(m_varCounter++));
        auto tmp     = ir_temp {tmpName};
        m_stack.push_back(tmp);
        return tmpName;
    }

private:
    int m_varCounter = 0;
    std::map<std::string, int> variables;
    std::vector<std::variant<int, ir_temp>> m_stack;
    std::vector<ir_statement> m_statements;
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
