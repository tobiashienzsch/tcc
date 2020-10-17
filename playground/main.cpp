#include "ast.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>

auto main(int /*argc*/, char ** /*argv*/) -> int {
  auto parser = Parser{"(1+2)*(2*(123+456))"};
  auto ast = parser.GenerateAST();

  parser.PrintDiagnostics(std::cout);
  ASTUtils::PrettyPrint(std::cout, *ast);

  return EXIT_SUCCESS;
}