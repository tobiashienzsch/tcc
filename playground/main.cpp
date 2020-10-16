#include "ast.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>

auto main(int /*argc*/, char ** /*argv*/) -> int {
  auto parser = Parser{"(42 - 143) + (123-456)"};
  auto expression = parser.Parse();

  parser.PrintDiagnostics(std::cout);
  ASTUtils::PrettyPrint(std::cout, *expression);

  return EXIT_SUCCESS;
}