#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/parser.hpp"

using TokenList = std::vector<tcc::SyntaxToken>;

constexpr auto source = std::string_view("1 + 2");
// constexpr auto source = std::string_view(R"(int main();)");
// constexpr auto source = std::string_view("0123 456 789 +-*/");
// constexpr auto source = std::string_view("int foo_bar() 888");

int main()
{
    auto parser  = tcc::Parser(source);
    auto& tokens = parser.GetTokens();

    for (auto const& token : tokens)
    {
        std::cout << token << '\n';
    }

    auto expression = parser.Parse();
    expression->Print();

    return EXIT_SUCCESS;
}