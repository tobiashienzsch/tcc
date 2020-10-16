#include "ast.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>

auto main(int /*argc*/, char ** /*argv*/) -> int {
  auto parser = Parser{"((1+2)*3)+143+(42/(2+1))"};
  auto expression = parser.Parse();

  parser.PrintDiagnostics(std::cout);
  ASTUtils::PrettyPrint(std::cout, *expression);

  return EXIT_SUCCESS;
}