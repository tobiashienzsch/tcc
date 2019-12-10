#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>

#include "tcc/tcc.hpp"

#include "tcc/compiler/lexer.hpp"

using TokenList = std::vector<tcc::SyntaxToken>;

// constexpr auto source = std::string_view("1 + 2");
// constexpr auto source = std::string_view(R"(int main();)");
// constexpr auto source = std::string_view("0123 456 789 +-*/");
constexpr auto source = std::string_view("int foo_bar() 888");
int main()
{
    auto lexer  = tcc::Lexer(source);
    auto type   = tcc::SyntaxToken::Type{};
    auto tokens = TokenList{};

    do
    {
        auto const token = lexer.GetNextToken();
        type             = token.type;
        if (token.type != tcc::SyntaxToken::Type::WhiteSpace && token.type != tcc::SyntaxToken::Type::Unknown)
        {
            tokens.push_back(token);
        }

    } while (type != tcc::SyntaxToken::Type::EndOfFile);

    for (auto const& token : tokens)
    {
        std::cout << token << '\n';
    }

    return EXIT_SUCCESS;
}