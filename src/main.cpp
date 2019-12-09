#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string_view>
#include <vector>

constexpr auto src = R"(
    int main();
)";

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
    enum class Type : int64_t
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
    int64_t const position;
    std::string_view const text;
};

class Lexer
{
public:
    explicit Lexer(std::string_view text) : m_text(text) {}

    SyntaxToken GetNextToken()
    {

        if (m_position >= m_text.size())
        {
            return SyntaxToken{SyntaxToken::Type::EndOfFile, m_position, std::string_view{}};
        }

        if (StringHelpers::CharIsDigit(current()))
        {
            auto const start = m_position;

            while (StringHelpers::CharIsDigit(current()))
            {
                next();
            }

            auto const length = m_position - start;
            auto const text = m_text.substr(start, length);

            return SyntaxToken{SyntaxToken::Type::Number, start, text};
        }

        if (StringHelpers::CharIsWhiteSpace(current()))
        {
            auto const start = m_position;

            while (StringHelpers::CharIsWhiteSpace(current()))
            {
                next();
            }

            auto const length = m_position - start;
            auto const text = m_text.substr(start, length);

            return SyntaxToken{SyntaxToken::Type::WhiteSpace, start, text};
        }

        if (current() == '+')
        {
            return SyntaxToken{SyntaxToken::Type::Plus, m_position++, "+"};
        }
        if (current() == '-')
        {
            return SyntaxToken{SyntaxToken::Type::Minus, m_position++, "-"};
        }
        if (current() == '*')
        {
            return SyntaxToken{SyntaxToken::Type::Star, m_position++, "*"};
        }
        if (current() == '/')
        {
            return SyntaxToken{SyntaxToken::Type::Slash, m_position++, "/"};
        }

        return SyntaxToken{SyntaxToken::Type::Unknown, m_position++, m_text.substr(m_position - 1, 1)};
    }

private:
    char current() const
    {
        if (m_position > m_text.size())
        {
            return '\0';
        }

        return m_text[m_position];
    }

    void next() { m_position++; }

private:
    int64_t m_position{0};
    std::string_view const m_text;
};

class Parser
{
public:
    Parser(std::string_view const source)
    {
        auto lexer = tcc::Lexer(source);
        auto type = tcc::SyntaxToken::Type{};

        do
        {
            auto const token = lexer.GetNextToken();
            type = token.type;
            if (token.type != SyntaxToken::Type::WhiteSpace && token.type != SyntaxToken::Type::Unknown)
            {
                m_tokens.push_back(token);
            }

        } while (type != SyntaxToken::Type::EndOfFile);
    }

    SyntaxToken Peek(int64_t offset)
    {
        auto const index = m_position + offset;
        if (index >= m_tokens.size())
        {
            return m_tokens.back();
        }

        return m_tokens.at(index);
    }

    SyntaxToken Current()
    {
        return Peek(0);
    }

    std::vector<SyntaxToken> m_tokens;

private:
    int64_t m_position{0};
};

std::ostream &operator<<(std::ostream &out, SyntaxToken::Type const type)
{
    switch (type)
    {
    case SyntaxToken::Type::Unknown:
        return out << "UNKNOWN";
    case SyntaxToken::Type::EndOfFile:
        return out << "EOF";
    case SyntaxToken::Type::WhiteSpace:
        return out << "WHITESPACE";
    case SyntaxToken::Type::Number:
        return out << "NUMBER";
    case SyntaxToken::Type::Plus:
        return out << "PLUS";
    case SyntaxToken::Type::Minus:
        return out << "MINUS";
    case SyntaxToken::Type::Star:
        return out << "STAR";
    case SyntaxToken::Type::Slash:
        return out << "SLASH";
    }
    return out << "";
}
} // namespace tcc

int main()
{
    auto srcView = std::string_view(src);
    // auto srcView = std::string_view("0123 456 789 +-*/");
    auto lexer = tcc::Lexer(srcView);

    auto parser = tcc::Parser{srcView};

    for (auto const &token : parser.m_tokens)
    {
        std::cout << token.type << ": " << token.position << '\n';
    }

    return EXIT_SUCCESS;
}