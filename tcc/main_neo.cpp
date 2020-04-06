#include <boost/lexical_cast.hpp>
#include <fstream>

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

  client::vmachine vm;            // Our virtual machine
  client::ast::FunctionList ast;  // Our AST

  client::ErrorHandler<IteratorType> errorHandler(iter, end);     // Our error handler
  client::parser::function<IteratorType> function(errorHandler);  // Our parser
  client::parser::skipper<IteratorType> skipper;                  // Our skipper
  client::code_gen::compiler compiler(errorHandler);              // Our compiler

  bool success = phrase_parse(iter, end, +function, skipper, ast);

  std::cout << "-------------------------\n";

  if (success && iter == end) {
    if (compiler(ast)) {
      boost::shared_ptr<client::code_gen::function> p = compiler.find_function("main");
      if (!p) return 1;

      int nargs = argc - 2;
      if (p->nargs() != nargs) {
        std::cerr << "Error: main function requires " << p->nargs() << " arguments." << std::endl;
        std::cerr << nargs << "supplied." << std::endl;
        return 1;
      }

      std::cout << "Success\n";
      std::cout << "-------------------------\n";
      std::cout << "Assembler----------------\n\n";
      compiler.print_assembler();

      // Push the arguments into our stack
      for (int i = 0; i < nargs; ++i) vm.get_stack()[i] = boost::lexical_cast<int>(argv[i + 2]);

      // Call the interpreter
      int r = vm.execute(compiler.get_code());

      std::cout << "-------------------------\n";
      std::cout << "Result: " << r << std::endl;
      std::cout << "-------------------------\n\n";
    } else {
      std::cout << "Compile failure\n";
    }
  } else {
    std::cout << "Parse failure\n";
  }
  return 0;
}
