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
        case SyntaxToken::Type::Percent: return out << "PERCENT";
        case SyntaxToken::Type::And: return out << "AND";
        case SyntaxToken::Type::Hash: return out << "HASH";
        case SyntaxToken::Type::ExclamationMark: return out << "EXCLAMATIONMARK";
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

        if (text == "auto") return SyntaxToken{SyntaxToken::Type::Auto, start, text};
        if (text == "return") return SyntaxToken{SyntaxToken::Type::Return, start, text};
        if (text == "if") return SyntaxToken{SyntaxToken::Type::If, start, text};

        return SyntaxToken{SyntaxToken::Type::Identifier, start, text};
    }

#define TCC_SINGLE_CHAR_SYNTAX_TOKEN(token, type)                                                                      \
    if (current() == token[0])                                                                                         \
    {                                                                                                                  \
        return SyntaxToken{SyntaxToken::Type::type, m_position++, token};                                              \
    }

    TCC_SINGLE_CHAR_SYNTAX_TOKEN("+", Plus);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("-", Minus);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("*", Star);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("/", Slash);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("%", Percent);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("&", And);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("#", Hash);
    TCC_SINGLE_CHAR_SYNTAX_TOKEN("!", ExclamationMark);

    return SyntaxToken{SyntaxToken::Type::Unknown, m_position++, m_text.substr(m_position - 1, 1)};
}
}  // namespace tcc