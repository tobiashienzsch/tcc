/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Now we'll introduce boolean expressions and control structures.
//  Is it obvious now what we are up to? ;-)
//
//  [ JDG April 9, 2007 ]       spirit2
//  [ JDG February 18, 2011 ]   Pure attributes. No semantic actions.
//  [ JDG June 6, 2014 ]        Ported from qi calc8 example.
//
///////////////////////////////////////////////////////////////////////////////

#include "ast.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "error_handler.hpp"
#include "printer.hpp"
#include "statement.hpp"
#include "vm.hpp"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main()
{

    std::string str;
    std::string source;
    // while (std::getline(std::cin, str))
    // {
    //     if (str.empty()) break;
    //     source += str + '\n';
    // }

    source = "var x = 1+2;\n\n";

    using client::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    client::vmachine vm;                // Our virtual machine
    client::code_gen::program program;  // Our VM program
    client::ast::statement_list ast;    // Our AST

    using boost::spirit::x3::with;
    using client::parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

    // Our compiler
    client::code_gen::compiler compile(program, error_handler);
    client::code_gen::ssa_builder ssaBuilder {};
    client::code_gen::printer printer(ssaBuilder, error_handler);

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<client::parser::error_handler_tag>(std::ref(error_handler))[client::statement()];

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success && iter == end)
    {
        // if (compile.start(ast))
        // {
        //     std::cout << "Success\n";
        //     std::cout << "-------------------------\n";
        //     vm.execute(program());

        //     std::cout << "-------------------------\n";
        //     std::cout << "Assembler----------------\n\n";
        //     program.print_assembler();

        //     std::cout << "-------------------------\n";
        //     std::cout << "Results------------------\n\n";
        //     program.print_variables(vm.get_stack());
        // }
        // else
        // {
        //     std::cout << "Compile failure\n";
        // }

        if (printer.start(ast))
        {
            for (auto& x : printer.ssaBuilder.statementList) std::cout << x.get() << '\n';
        }
        else
        {
            std::cout << "Printer failure\n";
        }
    }
    else
    {
        std::cout << "Parse failure\n";
    }

    return 0;
}
