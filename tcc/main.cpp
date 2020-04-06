#include <boost/lexical_cast.hpp>
#include <iterator>

#include "tcc/asm/assembly_generator.hpp"
#include "tcc/cmd/program_options.hpp"
#include "tcc/ir/generator.hpp"
#include "tcc/optimizer/optimizer.hpp"
#include "tcc/parser/function.hpp"
#include "tcc/parser/skipper.hpp"
#include "tsl/tsl.hpp"

int main(int argc, char** argv) {
  auto programOptions = tcc::ProgramOptions{};
  auto const [shouldExit, exitCode] = programOptions.ParseArguments(argc, argv);
  auto const flags = programOptions.GetCompilerFlags();
  if (shouldExit) {
    return exitCode;
  }

  using IteratorType = std::string::const_iterator;
  IteratorType iter = flags.Source.begin();
  IteratorType end = flags.Source.end();

  tcc::ErrorHandler<IteratorType> errorHandler(iter, end);         // Our error handler
  tcc::parser::Statement<IteratorType> statement(errorHandler);    // Our parser
  tcc::parser::Skipper<IteratorType> skipper;                      // Our skipper
  tcc::ast::StatementList ast;                                     // Our AST
  tcc::Program program{};                                          // Our VM program
  tcc::IntermediateRepresentation irBuilder;                       // IR builder
  tcc::IRGenerator irGenerator{program, irBuilder, errorHandler};  // IR Generator

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
