#pragma once

#include "ast.hpp"
#include "error_handler.hpp"
#include "ir/intermediate_representation.hpp"
#include "vm.hpp"

#include "tcc/core.hpp"

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
    compiler(tcc::code_gen::program& prog, tcc::IntermediateRepresentation& builder, ErrorHandler const& errorHandler)
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
    tcc::IntermediateRepresentation& m_builder;
    error_handler_type error_handler;
};
}  // namespace code_gen
}  // namespace tcc
