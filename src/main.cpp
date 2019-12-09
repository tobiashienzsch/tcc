#include <cstdio>
#include <cstdlib>

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

        NumberExpression,
        BinaryExpression,
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
    int64_t m_position{0};
    std::string_view const m_text;
};

class SyntaxNode
{
public:
    virtual ~SyntaxNode()               = default;
    virtual SyntaxToken::Type GetType() = 0;
};

class ExpressionSyntax : public SyntaxNode
{
};
class NumberExpressionSyntax : public ExpressionSyntax
{
public:
    NumberExpressionSyntax(SyntaxToken token) : m_token(token) {}
    ~NumberExpressionSyntax() override = default;
    SyntaxToken::Type GetType() override { return SyntaxToken::Type::NumberExpression; }

private:
    SyntaxToken m_token;
};

class BinaryExpressionSyntax : public ExpressionSyntax
{
public:
    BinaryExpressionSyntax(ExpressionSyntax const& left, ExpressionSyntax const& right, SyntaxToken const token)
        : m_left(left), m_right(right), m_operatorToken(token)
    {
    }

    ~BinaryExpressionSyntax() override = default;

    SyntaxToken::Type GetType() override { return SyntaxToken::Type::BinaryExpression; }

private:
    ExpressionSyntax const& m_left;
    ExpressionSyntax const& m_right;
    SyntaxToken const m_operatorToken;
};

class Parser
{
public:
    using TokenList = std::vector<SyntaxToken>;

public:
    Parser(std::string_view const source)
    {
        auto lexer = tcc::Lexer(source);
        auto type  = tcc::SyntaxToken::Type{};

        do
        {
            auto const token = lexer.GetNextToken();
            type             = token.type;
            if (token.type != SyntaxToken::Type::WhiteSpace && token.type != SyntaxToken::Type::Unknown)
            {
                m_tokens.push_back(token);
            }

        } while (type != SyntaxToken::Type::EndOfFile);
    }

    ExpressionSyntax* Parse()
    {
        auto const current = currentToken();
        auto left          = parsePrimaryExpression();

        while (current.type == SyntaxToken::Type::Plus || current.type == SyntaxToken::Type::Minus)
        {
            auto const operatorToken = nextToken();
            auto const right         = parsePrimaryExpression();
            left                     = new BinaryExpressionSyntax{*left, *right, operatorToken};
        }

        return left;
    }

    TokenList& GetTokens() { return m_tokens; }

private:
    ExpressionSyntax* parsePrimaryExpression()
    {
        auto const numberToken = matchTokenType(SyntaxToken::Type::Number);
        return new NumberExpressionSyntax{numberToken};
    }

    SyntaxToken matchTokenType(SyntaxToken::Type type)
    {
        auto const current = currentToken();
        if (current.type == type)
        {
            return nextToken();
        }

        return SyntaxToken{type, current.position, ""};
    }

    SyntaxToken nextToken()
    {
        auto const current = currentToken();
        m_position++;
        return current;
    }

    SyntaxToken peekToken(int64_t offset)
    {
        auto const index = m_position + offset;
        if (index >= m_tokens.size())
        {
            return m_tokens.back();
        }

        return m_tokens.at(index);
    }

    SyntaxToken currentToken() { return peekToken(0); }

private:
    TokenList m_tokens;
    int64_t m_position{0};
};

std::ostream& operator<<(std::ostream& out, SyntaxToken::Type const type)
{
    switch (type)
    {
        case SyntaxToken::Type::Unknown: return out << "UNKNOWN";
        case SyntaxToken::Type::EndOfFile: return out << "EOF";
        case SyntaxToken::Type::WhiteSpace: return out << "WHITESPACE";
        case SyntaxToken::Type::Number: return out << "NUMBER";
        case SyntaxToken::Type::Plus: return out << "PLUS";
        case SyntaxToken::Type::Minus: return out << "MINUS";
        case SyntaxToken::Type::Star: return out << "STAR";
        case SyntaxToken::Type::Slash: return out << "SLASH";
    }

    return out << "";
}
}  // namespace tcc

int main()
{
    // auto srcView = std::string_view(src);
    auto srcView = std::string_view("0123 456 789 +-*/");
    auto parser  = tcc::Parser{srcView};

    for (auto const& token : parser.GetTokens())
    {
        std::cout << token.type << ": " << token.position << '\n';
    }

    auto* parseTree = parser.Parse();
    delete parseTree;

    return EXIT_SUCCESS;
}