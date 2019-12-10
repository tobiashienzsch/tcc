#pragma once

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

namespace tcc
{

class StringHelpers
{
public:
    static constexpr bool CharIsDigit(char c) noexcept { return (c > 47 && c < 58); }
    static constexpr bool CharIsWhiteSpace(char c) noexcept { return (c == ' ' || c == '\n' || c == '\r'); }
};

class SyntaxToken
{
public:
    enum class Type
    {
        Unknown,
        EndOfFile,
        WhiteSpace,

        Number,
        Plus,
        Minus,
        Star,
        Slash,

    };

    Type const type;
    uint64_t const position;
    std::string_view const text;
};

std::ostream& operator<<(std::ostream& out, SyntaxToken::Type type);
std::ostream& operator<<(std::ostream& out, SyntaxToken token);

class Lexer
{
public:
    explicit Lexer(std::string_view text) : m_text(text) {}

    SyntaxToken GetNextToken();

private:
    char current() const noexcept
    {
        if (m_position > m_text.size())
        {
            return '\0';
        }

        return m_text[m_position];
    }

    void next() noexcept { m_position++; }

private:
    uint64_t m_position{0};
    std::string_view const m_text;
};

}  // namespace tcc