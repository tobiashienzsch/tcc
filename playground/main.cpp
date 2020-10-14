#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

enum class SyntaxTokenType
{
    BadToken = 0,
    Whitespace,
    LiteralInteger,
    Identifier,
    Plus,
    Minus,
    Star,
    Slash,
    OpenBrace,
    CloseBrace,
    EndOfFile,
};

std::ostream& operator<<(std::ostream& out, SyntaxTokenType const type)
{
    switch (type)
    {
        case SyntaxTokenType::BadToken:
        {
            out << "BadToken";
            break;
        }
        case SyntaxTokenType::Whitespace:
        {
            out << "Whitespace";
            break;
        }
        case SyntaxTokenType::LiteralInteger:
        {
            out << "LiteralInteger";
            break;
        }
        case SyntaxTokenType::Identifier:
        {
            out << "Identifier";
            break;
        }
        case SyntaxTokenType::Plus:
        {
            out << "Plus";
            break;
        }
        case SyntaxTokenType::Minus:
        {
            out << "Minus";
            break;
        }
        case SyntaxTokenType::Star:
        {
            out << "Star";
            break;
        }
        case SyntaxTokenType::Slash:
        {
            out << "Slash";
            break;
        }

        case SyntaxTokenType::OpenBrace:
        {
            out << "OpenBrace";
            break;
        }
        case SyntaxTokenType::CloseBrace:
        {
            out << "CloseBrace";
            break;
        }

        case SyntaxTokenType::EndOfFile:
        {
            out << "EndOfFile";
            break;
        }
    }

    return out;
}

struct SyntaxToken
{
    SyntaxTokenType Type;
    std::size_t Position;
    std::string Text;
};

class Lexer
{
public:
    Lexer(std::string text) : text_ {std::move(text)} { }

    auto NextToken() -> SyntaxToken
    {
        if (position_ >= text_.size())
        {
            return SyntaxToken {
                .Type     = SyntaxTokenType::EndOfFile,
                .Position = position_,
                .Text     = "\0",
            };
        }
        // Whitespace
        if (std::isspace(current()))
        {
            auto const start = position_;
            while (std::isspace(current()))
            {
                next();
            }

            auto const length = position_ - start;

            return SyntaxToken {
                .Type     = SyntaxTokenType::Whitespace,
                .Position = start,
                .Text     = text_.substr(start, length),
            };
        }
        // LiteralInteger
        if (std::isdigit(current()))
        {
            auto const start = position_;
            while (std::isdigit(current()))
            {
                next();
            }

            auto const length = position_ - start;
            return SyntaxToken {
                .Type     = SyntaxTokenType::LiteralInteger,
                .Position = start,
                .Text     = text_.substr(start, length),
            };
        }

        if (current() == '+')
        {
            return {SyntaxTokenType::Plus, position_++, "+"};
        }

        if (current() == '-')
        {
            return {SyntaxTokenType::Minus, position_++, "-"};
        }

        if (current() == '*')
        {
            return {SyntaxTokenType::Star, position_++, "*"};
        }

        if (current() == '/')
        {
            return {SyntaxTokenType::Slash, position_++, "/"};
        }

        if (current() == '(')
        {
            return {SyntaxTokenType::OpenBrace, position_++, "("};
        }

        if (current() == ')')
        {
            return {SyntaxTokenType::CloseBrace, position_++, ")"};
        }

        return SyntaxToken {
            .Type     = SyntaxTokenType::BadToken,
            .Position = position_++,
            .Text     = text_.substr(position_ - 1, 1),
        };
    }

private:
    auto current() -> char { return position_ >= text_.size() ? '\0' : text_[position_]; }

    auto next() -> std::size_t { return ++position_; }

    std::string const text_;
    std::size_t position_ {0};
};

enum class ASTNodeType
{
    Token,
    ConstantExpression,
    BinaryExpression,
};

std::ostream& operator<<(std::ostream& out, ASTNodeType const type)
{
    switch (type)
    {
        case ASTNodeType::Token:
        {
            out << "Token";
            break;
        }
        case ASTNodeType::ConstantExpression:
        {
            out << "ConstantExpr";
            break;
        }
        case ASTNodeType::BinaryExpression:
        {
            out << "BinaryExpr";
            break;
        }
    }
    return out;
}

class ASTNode
{
public:
    virtual ~ASTNode() = default;

    virtual ASTNodeType GetType() const                                   = 0;
    virtual std::vector<ASTNode*> GetChildren() const                     = 0;
    virtual std::pair<std::size_t, std::size_t> GetSourceLocation() const = 0;
};

class ASTToken final : public ASTNode
{
public:
    ASTToken(SyntaxToken token) : token_ {std::move(token)} { }
    ASTToken(ASTToken const&) = delete;
    ASTToken& operator=(ASTToken const&) = delete;
    ASTToken(ASTToken&&) noexcept        = default;
    ASTToken& operator=(ASTToken&&) noexcept = default;
    ~ASTToken() override                     = default;

    ASTNodeType GetType() const override { return ASTNodeType::Token; }

    std::vector<ASTNode*> GetChildren() const override { return {}; }

    std::pair<std::size_t, std::size_t> GetSourceLocation() const override
    {
        return std::make_pair(token_.Position, token_.Position + token_.Text.size());
    }

    SyntaxToken Token() const noexcept { return token_; }

private:
    SyntaxToken token_;
};

class ASTConstantExpr final : public ASTNode
{
public:
    ASTConstantExpr(SyntaxToken token) : token_ {std::make_unique<ASTToken>(std::move(token))} { }
    ASTConstantExpr(ASTConstantExpr const&) = delete;
    ASTConstantExpr& operator=(ASTConstantExpr const&) = delete;
    ASTConstantExpr(ASTConstantExpr&&) noexcept        = default;
    ASTConstantExpr& operator=(ASTConstantExpr&&) noexcept = default;
    ~ASTConstantExpr() override                            = default;

    ASTNodeType GetType() const override { return ASTNodeType::ConstantExpression; }

    std::vector<ASTNode*> GetChildren() const override { return {token_.get()}; }

    std::pair<std::size_t, std::size_t> GetSourceLocation() const override
    {
        return token_->GetSourceLocation();
    }

private:
    std::unique_ptr<ASTToken> token_;
};

class ASTBinaryExpr final : public ASTNode
{
public:
    ASTBinaryExpr(std::unique_ptr<ASTNode> lhs, SyntaxToken op, std::unique_ptr<ASTNode> rhs)
        : lhs_ {std::move(lhs)}
        , operator_ {std::make_unique<ASTToken>(std::move(op))}
        , rhs_ {std::move(rhs)}
    {
    }
    ASTBinaryExpr(ASTBinaryExpr const&) = delete;
    ASTBinaryExpr& operator=(ASTBinaryExpr const&) = delete;
    ASTBinaryExpr(ASTBinaryExpr&&) noexcept        = default;
    ASTBinaryExpr& operator=(ASTBinaryExpr&&) noexcept = default;

    ~ASTBinaryExpr() override = default;

    ASTNodeType GetType() const override { return ASTNodeType::BinaryExpression; }

    std::vector<ASTNode*> GetChildren() const override
    {
        auto res = std::vector<ASTNode*> {lhs_.get(), operator_.get(), rhs_.get()};
        return res;
    }

    std::pair<std::size_t, std::size_t> GetSourceLocation() const override
    {
        auto const start = lhs_->GetSourceLocation().first;
        auto const end   = rhs_->GetSourceLocation().second;
        return std::make_pair(start, end);
    }

private:
    std::unique_ptr<ASTNode> lhs_;
    std::unique_ptr<ASTToken> operator_;
    std::unique_ptr<ASTNode> rhs_;
};

class Parser
{
public:
    Parser(std::string text)
    {
        auto lexer = Lexer {std::move(text)};
        auto token = lexer.NextToken();
        for (; token.Type != SyntaxTokenType::EndOfFile; token = lexer.NextToken())
        {
            if (token.Type != SyntaxTokenType::Whitespace && token.Type != SyntaxTokenType::BadToken)
            {
                tokens_.push_back(token);
            }
        }
    }

    auto Parse() -> std::unique_ptr<ASTNode>
    {
        auto lhs = parsePrimaryExpression();

        while (current().Type == SyntaxTokenType::Plus || current().Type == SyntaxTokenType::Minus)
        {
            auto op  = nextToken();
            auto rhs = parsePrimaryExpression();
            lhs      = std::make_unique<ASTBinaryExpr>(std::move(lhs), op, std::move(rhs));
        }

        return lhs;
    }

private:
    [[nodiscard]] auto parsePrimaryExpression() -> std::unique_ptr<ASTNode>
    {
        auto numberToken = match(SyntaxTokenType::LiteralInteger);
        return std::make_unique<ASTConstantExpr>(numberToken);
    }

    [[nodiscard]] auto match(SyntaxTokenType type) noexcept -> SyntaxToken
    {
        if (current().Type == type)
        {
            return nextToken();
        }

        return SyntaxToken {type, current().Position, ""};
    }

    [[nodiscard]] auto nextToken() noexcept -> SyntaxToken
    {
        auto cur = current();
        position_++;
        return cur;
    }

    [[nodiscard]] auto peek(size_t offset) const noexcept -> SyntaxToken
    {
        auto const index    = position_ + offset;
        auto const maxIndex = std::max<size_t>(tokens_.size() - 1, 0);
        return tokens_.at(std::min(index, maxIndex));
    }

    [[nodiscard]] auto current() const noexcept -> SyntaxToken { return peek(0); }

    std::size_t position_ {};
    std::vector<SyntaxToken> tokens_;
};

auto PrettyPrint(std::ostream& out, ASTNode const& node, std::string indent = "", bool isLast = true)
    -> void
{
    auto const marker = isLast ? std::string("└── ") : std::string("├── ");

    out << indent;
    out << marker;
    out << node.GetType() << "[" << node.GetSourceLocation().first << ", "
        << node.GetSourceLocation().second << ") ";

    if (auto const* ptr = dynamic_cast<ASTToken const*>(&node); ptr != nullptr)
    {
        out << ptr->Token().Type << " \"" << ptr->Token().Text << "\"";
    }
    out << '\n';

    indent += isLast ? std::string("    ") : std::string("│   ");
    auto const children = node.GetChildren();
    if (not children.empty())
    {
        std::for_each(children.begin(), children.end() - 1,
                      [&](auto const& child) { PrettyPrint(out, *child, indent, false); });
        PrettyPrint(out, *children.back(), indent, true);
    }
}

int main()
{
    auto parser     = Parser {R"(42 + 54 + 143 - 20)"};
    auto expression = parser.Parse();
    PrettyPrint(std::cout, *expression);

    return 0;
}