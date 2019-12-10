#include "lexer.hpp"

namespace tcc
{
SyntaxToken Lexer::GetNextToken()
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
        auto const text   = m_text.substr(start, length);

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
        auto const text   = m_text.substr(start, length);

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
}  // namespace tcc