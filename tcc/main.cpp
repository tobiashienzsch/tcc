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
  if (shouldExit) {
    return exitCode;
  }

  using IteratorType = std::string::const_iterator;
  auto const flags = programOptions.GetCompilerFlags();
  auto iter = flags.Source.begin();
  auto end = flags.Source.end();

  auto errorHandler = tcc::ErrorHandler<IteratorType>{iter, end};     // Our error handler
  auto function = tcc::parser::Function<IteratorType>{errorHandler};  // Our parser
  auto skipper = tcc::parser::Skipper<IteratorType>{};                // Our skipper
  auto ast = tcc::ast::Function{};                                    // Our AST
  auto irGenerator = tcc::IRGenerator{errorHandler};                  // IR Generator

  bool success = phrase_parse(iter, end, function, skipper, ast);
  if (!success || iter != end) {
    fmt::print("Error while parsing!\n");
    return EXIT_FAILURE;
  }

  if (!irGenerator(ast.body)) {
    fmt::print("Error while compiling!\n");
    return EXIT_FAILURE;
  }

  if (flags.OptLevel > 0) {
    auto optimizer = tcc::Optimizer(*irGenerator.GetBuilder().CurrentScope());
    optimizer.Optimize();
  }

  if (flags.PrintIR) {
    irGenerator.GetBuilder().PrintIR();
  }

  if (!flags.OutputName.empty()) {
    auto assembly = tcc::AssemblyGenerator::Build(*irGenerator.GetBuilder().CurrentScope());
    auto binaryProgram = tcc::BinaryProgram{1, flags.OutputName, 0, assembly};
    if (!tcc::BinaryFormat::WriteToFile(flags.OutputName, binaryProgram)) {
      fmt::print("Error while writing binary!\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
