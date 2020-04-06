#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "tcc/asm/assembly_generator.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser-qi/function.hpp"
#include "tcc/parser-qi/skipper.hpp"
#include "tsl/tsl.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
  std::string source = R"(
    int a = 1 + (2 + 8 / 2) * 3;
    int b = 1 + (2 + 8 / 2) * 3;
    
    int x = 1 + (2 + 8 / 2) * 3;
    x = x * 2;
    int y = x + 2 * 2;
    return y * 2;
  )";

  struct CompilerFlags {
    int OptLevel = 0;
    bool PrintSource = false;
    bool PrintAst = false;
    bool PrintIR = true;
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

      std::ifstream in(paths[0], std::ios_base::in);
      if (!in) {
        std::cerr << "Error: Could not open input file: " << paths[0] << std::endl;
        return 1;
      }

      in.unsetf(std::ios::skipws);  // No white space skipping!
      source = "";
      std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(source));
    }

  } catch (std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Exception of unknown type!\n";
  }

  using IteratorType = std::string::const_iterator;
  IteratorType iter = source.begin();
  IteratorType end = source.end();

  client::ErrorHandler<IteratorType> errorHandler(iter, end);       // Our error handler
  client::parser::Statement<IteratorType> statement(errorHandler);  // Our parser
  client::parser::Skipper<IteratorType> skipper;                    // Our skipper
  client::ast::StatementList ast;                                   // Our AST
  tcc::Program program{};                                           // Our VM program
  tcc::IntermediateRepresentation irBuilder;                        // IR builder
  tcc::IRGenerator irGenerator{program, irBuilder, errorHandler};   // IR Generator

  bool success = phrase_parse(iter, end, statement, skipper, ast);
  if (!success || iter != end) {
    std::cout << "Parse error!\n";
    return EXIT_FAILURE;
  }

  // Compile IR
  if (!irGenerator(ast)) {
    std::cout << "Compile error!\n";
    return EXIT_FAILURE;
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
