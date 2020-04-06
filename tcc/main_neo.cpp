#include <boost/lexical_cast.hpp>
#include <fstream>

#include "tcc/ir/generator.hpp"
#include "tcc/parser-qi/compiler.hpp"
#include "tcc/parser-qi/function.hpp"
#include "tcc/parser-qi/skipper.hpp"
#include "tcc/parser-qi/vm.hpp"

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  char const* filePath;
  if (argc > 1) {
    filePath = argv[1];
  } else {
    std::cerr << "Error: No input file provided." << std::endl;
    return 1;
  }

  std::ifstream in(filePath, std::ios_base::in);

  if (!in) {
    std::cerr << "Error: Could not open input file: " << filePath << std::endl;
    return 1;
  }

  std::string sourceCode;       // We will read the contents here.
  in.unsetf(std::ios::skipws);  // No white space skipping!
  std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(sourceCode));

  using IteratorType = std::string::const_iterator;
  IteratorType iter = sourceCode.begin();
  IteratorType end = sourceCode.end();

  client::vmachine vm;             // Our virtual machine
  client::ast::StatementList ast;  // Our AST

  client::ErrorHandler<IteratorType> errorHandler(iter, end);       // Our error handler
  client::parser::Statement<IteratorType> statement(errorHandler);  // Our parser
  client::parser::Skipper<IteratorType> skipper;                    // Our skipper
  client::code_gen::Compiler compiler(errorHandler);                // Our compiler
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

  return 0;
}
