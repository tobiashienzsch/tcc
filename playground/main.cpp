#include "ast.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>

auto main(int /*argc*/, char ** /*argv*/) -> int {
  auto parser = Parser{"(((123+(456-123))+(123+123+1234)))"};
  auto expression = parser.Parse();

  parser.PrintDiagnostics(std::cout);
  ASTUtils::PrettyPrint(std::cout, *expression);

  return EXIT_SUCCESS;
}