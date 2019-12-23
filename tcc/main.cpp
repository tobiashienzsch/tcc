#include "asm/assembly_generator.hpp"
#include "ast.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "error_handler.hpp"
#include "optimizer/optimizer.hpp"
#include "statement.hpp"
#include "vm.hpp"

#include "tcc/core/binary_format.hpp"

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

    using tcc::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    tcc::vmachine vm;                           // Our virtual machine
    tcc::code_gen::program program;             // Our VM program
    tcc::IntermediateRepresentation irBuilder;  // IR builder
    tcc::ast::statement_list ast;               // Our AST

    using boost::spirit::x3::with;
    using tcc::parser::error_handler_type;
    error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

    // Our compiler
    tcc::code_gen::compiler compile(program, irBuilder, error_handler);

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<tcc::parser::error_handler_tag>(std::ref(error_handler))[tcc::statement()];

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success && iter == end)
    {
        if (compile.start(ast))
        {
            vm.execute(program());
            program.print_variables(vm.get_stack());

            auto optimizer = tcc::Optimizer(*irBuilder.CurrentScope());
            optimizer.Optimize();
            auto assembly      = tcc::AssemblyGenerator::Build(*irBuilder.CurrentScope());
            auto binaryProgram = tcc::BinaryProgram{1, "test", 0, assembly};
            if (!tcc::BinaryFormat::WriteToFile("test.tcb", binaryProgram)) fmt::print("Error wrtiting binary.\n");
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
