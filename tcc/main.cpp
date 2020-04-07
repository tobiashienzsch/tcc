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

  tcc::ErrorHandler<IteratorType> errorHandler(iter, end);     // Our error handler
  tcc::parser::Function<IteratorType> function(errorHandler);  // Our parser
  tcc::parser::Skipper<IteratorType> skipper;                  // Our skipper
  tcc::ast::Function ast;                                      // Our AST
  tcc::IntermediateRepresentation irBuilder;                   // IR builder
  tcc::IRGenerator irGenerator{irBuilder, errorHandler};       // IR Generator

  bool success = phrase_parse(iter, end, function, skipper, ast);
  if (!success || iter != end) {
    fmt::print("Parse error!\n");
    return EXIT_FAILURE;
  }

  // Compile IR
  if (!irGenerator(ast.body)) {
    fmt::print("Compile error!\n");
    return EXIT_FAILURE;
  }

  if (flags.OptLevel > 0) {
    auto optimizer = tcc::Optimizer(*irBuilder.CurrentScope());
    optimizer.Optimize();
  }

  if (flags.PrintIR) {
    irBuilder.PrintIR();
  }

  if (!flags.OutputName.empty()) {
    auto assembly = tcc::AssemblyGenerator::Build(*irBuilder.CurrentScope());
    auto binaryProgram = tcc::BinaryProgram{1, "test", 0, assembly};
    if (!tcc::BinaryFormat::WriteToFile(flags.OutputName, binaryProgram)) fmt::print("Error wrtiting binary.\n");
  }

  return EXIT_SUCCESS;
}
