#include "lexer.hpp"

namespace tcc
{
std::ostream& operator<<(std::ostream& out, SyntaxToken::Type const type)
{
    switch (type)
    {
        case SyntaxToken::Type::Unknown: return out << "UNKNOWN";
        case SyntaxToken::Type::EndOfFile: return out << "EOF";
        case SyntaxToken::Type::WhiteSpace: return out << "WHITESPACE";

        case SyntaxToken::Type::Number: return out << "NUMBER";
        case SyntaxToken::Type::Identifier: return out << "IDENTIFIER";

        case SyntaxToken::Type::Plus: return out << "PLUS";
        case SyntaxToken::Type::Minus: return out << "MINUS";
        case SyntaxToken::Type::Star: return out << "STAR";
        case SyntaxToken::Type::Slash: return out << "SLASH";
    }

    return out << "";
}

std::ostream& operator<<(std::ostream& out, SyntaxToken const token) { return out << "TOKEN::" << token.type; }

SyntaxToken Lexer::GetNextToken()
{

    if (m_position >= m_text.size())
    {
        return SyntaxToken{SyntaxToken::Type::EndOfFile, m_position, std::string_view{}};
    }

    if (StringHelpers::CharIsWhiteSpace(current()))
    {
        auto const start = m_position;

        do
        {
            next();
            if (m_position >= m_text.size()) break;
        } while (StringHelpers::CharIsWhiteSpace(current()));

        auto const length = m_position - start;
        auto const text   = m_text.substr(start, length);

        return SyntaxToken{SyntaxToken::Type::WhiteSpace, start, text};
    }

    if (StringHelpers::CharIsDigit(current()))
    {
        auto const start = m_position;

        do
        {
            next();
            if (m_position >= m_text.size()) break;
        } while (StringHelpers::CharIsDigit(current()));

        auto const length = m_position - start;
        auto const text   = m_text.substr(start, length);

        return SyntaxToken{SyntaxToken::Type::Number, start, text};
    }

    if (StringHelpers::CharIsAlphabetical(current()) || current() == '_')
    {
        auto const start = m_position;

        do
        {
            next();
            if (m_position >= m_text.size()) break;
        } while (StringHelpers::CharIsAlphaNumeric(current()));

        auto const length = m_position - start;
        auto const text   = m_text.substr(start, length);

        return SyntaxToken{SyntaxToken::Type::Identifier, start, text};
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