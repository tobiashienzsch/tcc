#include "tcc/asm/assembly_generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/ast.hpp"
#include "tcc/parser/ast_printer.hpp"
#include "tcc/parser/compiler.hpp"
#include "tcc/parser/config.hpp"
#include "tcc/parser/error_handler.hpp"
#include "tcc/parser/statement.hpp"
#include "tcc/vm.hpp"

//
#include "tsl/tsl.hpp"

//
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <fstream>
#include <iostream>
#include <iterator>

auto readSourceFile(std::string path) -> std::string {
  std::ifstream f(path);
  std::string str;

  if (f) {
    std::ostringstream ss;
    ss << f.rdbuf();  // reading data
    str = ss.str();
  }
  return str;
}

auto main(int argc, char** argv) -> int {
  // return = 84
  std::string source = R"(
    auto a = 1 + (2 + 8 / 2) * 3;
    auto b = 1 + (2 + 8 / 2) * 3;
    
    auto x = 1 + (2 + 8 / 2) * 3;
    x = x * 2;
    auto y = x + 2 * 2;
    return y * 2;
  )";

  struct CompilerFlags {
    int OptLevel = 0;
    bool PrintSource = false;
    bool PrintAst = false;
    bool PrintIR = false;
  } flags;

  try {
    // Declare a group of options that will be
    // allowed only on command line
    po::options_description desc("Tobante's Crappy Compiler");
    desc.add_options()                                                                 //
        ("help,h", "produce this help message")                                        //
        ("input,i", po::value<std::vector<std::string>>(), "input source file")        //
        ("optimization,O", po::value<int>(&flags.OptLevel), "optimization level 0-1")  //
        ("print-source", po::bool_switch(&flags.PrintSource), "print source code")     //
        ("print-ast", po::bool_switch(&flags.PrintAst), "print parsed ast")            //
        ("print-ir", po::bool_switch(&flags.PrintIR), "print generated ir")            //
        ;

    po::positional_options_description p;
    p.add("input", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    }

    if (vm.count("input")) {
      auto const paths = vm["input"].as<std::vector<std::string>>();
      if (paths.size() > 1) {
        fmt::print("Only one source file allowed currently.\n");
        return EXIT_FAILURE;
      }
      source = readSourceFile(paths[0]);
    }

  } catch (std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Exception of unknown type!\n";
  }

  using tcc::parser::iterator_type;
  iterator_type iter(source.begin());
  iterator_type end(source.end());
  if (flags.PrintSource) {
    fmt::print("Source:\n{}\n\n", source);
  }

  // tcc::vmachine vm;                           // Our virtual machine
  tcc::code_gen::program program;             // Our VM program
  tcc::IntermediateRepresentation irBuilder;  // IR builder
  tcc::ast::StatementList ast;                // Our AST

  using boost::spirit::x3::with;
  using tcc::parser::error_handler_type;
  error_handler_type error_handler(iter, end, std::cerr);  // Our error handler

  // Our compiler
  tcc::code_gen::compiler compile(program, irBuilder, error_handler);

  // Our parser
  // We pass our error handler to the parser so we can access
  // it later on in our on_error and on_sucess handlers
  auto const parser = with<tcc::parser::error_handler_tag>(std::ref(error_handler))[tcc::GetStatement()];

  using boost::spirit::x3::ascii::space;

  // Parse
  bool success = phrase_parse(iter, end, parser, space, ast);
  if (!success || iter != end) {
    std::cout << "Parse failure\n";
    return EXIT_FAILURE;
  }

  // Compile IR
  if (!compile.start(ast)) {
    std::cout << "Compile failure\n";
    return EXIT_FAILURE;
  }

  // Print AST
  if (flags.PrintAst) {
    auto printer = tcc::parser::AstPrinter{error_handler};
    if (!printer.start(ast)) {
      std::cout << "Ast printer failure\n";
      return EXIT_FAILURE;
    }
  }
  if (flags.OptLevel > 0) {
    auto optimizer = tcc::Optimizer(*irBuilder.CurrentScope());
    optimizer.Optimize();
  }

  if (flags.PrintIR) {
    irBuilder.PrintIR();
  }

  auto assembly = tcc::AssemblyGenerator::Build(*irBuilder.CurrentScope());
  auto binaryProgram = tcc::BinaryProgram{1, "test", 0, assembly};
  if (!tcc::BinaryFormat::WriteToFile("test.tcb", binaryProgram)) fmt::print("Error wrtiting binary.\n");

  return 0;
}
