#include "ast.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <iostream>

int main() {
  auto parser = Parser{" + 143"};
  auto expression = parser.Parse();

  parser.PrintDiagnostics(std::cout);
  ASTUtils::PrettyPrint(std::cout, *expression);

  return EXIT_SUCCESS;
}