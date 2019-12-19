#include "ast.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "error_handler.hpp"
#include "statement.hpp"
#include "vm.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <fstream>
#include <iostream>
#include <iterator>

auto readSourceFile(std::string path) -> std::string
{
    std::ifstream f(path);
    std::string str;

    if (f)
    {
        std::ostringstream ss;
        ss << f.rdbuf();  // reading data
        str = ss.str();
    }
    return str;
}

auto main(int argc, char** argv) -> int
{
    std::string source;
    source = "auto x=4*(1+2)*5; auto y=x+3;\n\n";
    source = "auto y=(1-2*3)*(7+3-1);\n\n";
    source = "auto y=(3)*(7+3-1); auto z = y+2;\n\n";
    source = "auto x = 1+(2+7*8/2)*3;x=x*2;auto y=x+2*2;\n\n";

    try
    {
        po::options_description desc("tcc: tobante's crappy compiler");
        desc.add_options()                                      //
            ("help,h", "produce this help message")             //
            ("file,f", po::value<std::string>(), "input file")  //
            ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("file"))
        {
            auto const path = vm["file"].as<std::string>();
            source          = readSourceFile(path);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Exception of unknown type!\n";
    }

    using client::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    client::vmachine vm;                  // Our virtual machine
    client::code_gen::program program;    // Our VM program
    client::code_gen::IRBuilder builder;  // IR builder
    client::ast::statement_list ast;      // Our AST

    using boost::spirit::x3::with;
    using client::parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

    // Our compiler
    client::code_gen::compiler compile(program, builder, error_handler);

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<client::parser::error_handler_tag>(std::ref(error_handler))[client::statement()];

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success && iter == end)
    {
        if (compile.start(ast))
        {
            builder.PrintStatementList();
            // std::cout << "Success\n";
            // std::cout << "-------------------------\n";
            vm.execute(program());

            // std::cout << "-------------------------\n";
            // std::cout << "Assembler----------------\n\n";
            // program.print_assembler();

            // std::cout << "-------------------------\n";
            // std::cout << "Results------------------\n\n";
            program.print_variables(vm.get_stack());
        }
        else
        {
            std::cout << "Compile failure\n";
        }

    }
    else
    {
        std::cout << "Parse failure\n";
    }

    return 0;
}
