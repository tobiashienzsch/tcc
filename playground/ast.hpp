#ifndef TCC_PLAYGROUND_AST_HPP
#define TCC_PLAYGROUND_AST_HPP

#include "token.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

enum class ASTNodeType {
  Operator,
  Constant,
  BinaryExpression,
  UnaryExpression,
  BracedExpression,
};

auto operator<<(std::ostream &out, ASTNodeType type) -> std::ostream &;

class ASTNode {
public:
  virtual ~ASTNode() = default;

  [[nodiscard]] virtual ASTNodeType GetType() const = 0;
  [[nodiscard]] virtual std::string GetSource() const = 0;
  [[nodiscard]] virtual std::vector<ASTNode *> GetChildren() const = 0;
  [[nodiscard]] virtual std::pair<std::size_t, std::size_t>
  GetSourceLocation() const = 0;
};

class ASTOperator final : public ASTNode {
public:
  ASTOperator(SyntaxToken token) : token_{std::move(token)} {}
  ASTOperator(ASTOperator const &) = delete;
  ASTOperator &operator=(ASTOperator const &) = delete;
  ASTOperator(ASTOperator &&) noexcept = default;
  ASTOperator &operator=(ASTOperator &&) noexcept = default;
  ~ASTOperator() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::Operator;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    return {};
  }

  [[nodiscard]] auto GetSource() const -> std::string override {
    return token_.Text;
  }

  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    return std::make_pair(token_.Position,
                          token_.Position + token_.Text.size());
  }

  [[nodiscard]] auto Token() const noexcept -> SyntaxToken { return token_; }

private:
  SyntaxToken token_;
};

class ASTConstant final : public ASTNode {
public:
  ASTConstant(SyntaxToken token) : token_{std::move(token)} {}
  ASTConstant(ASTConstant const &) = delete;
  ASTConstant &operator=(ASTConstant const &) = delete;
  ASTConstant(ASTConstant &&) noexcept = default;
  ASTConstant &operator=(ASTConstant &&) noexcept = default;
  ~ASTConstant() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::Constant;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    return {};
  }
  [[nodiscard]] auto GetSource() const -> std::string override {
    return token_.Text;
  }
  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    return std::make_pair(token_.Position,
                          token_.Position + token_.Text.size());
  }

  [[nodiscard]] auto Token() const noexcept -> SyntaxToken { return token_; }

private:
  SyntaxToken token_;
};

class ASTBinaryExpr final : public ASTNode {
public:
  ASTBinaryExpr(std::unique_ptr<ASTNode> lhs, SyntaxToken op,
                std::unique_ptr<ASTNode> rhs)
      : lhs_{std::move(lhs)},
        operator_{std::make_unique<ASTOperator>(std::move(op))}, rhs_{std::move(
                                                                     rhs)} {}
  ASTBinaryExpr(ASTBinaryExpr const &) = delete;
  ASTBinaryExpr &operator=(ASTBinaryExpr const &) = delete;
  ASTBinaryExpr(ASTBinaryExpr &&) noexcept = default;
  ASTBinaryExpr &operator=(ASTBinaryExpr &&) noexcept = default;

  ~ASTBinaryExpr() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::BinaryExpression;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    auto res = std::vector<ASTNode *>{lhs_.get(), operator_.get(), rhs_.get()};
    return res;
  }

  [[nodiscard]] auto GetSource() const -> std::string override {
    return fmt::format("{} {} {}", lhs_->GetSource(), operator_->GetSource(),
                       rhs_->GetSource());
  }

  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = lhs_->GetSourceLocation().first;
    auto const end = rhs_->GetSourceLocation().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ASTNode> lhs_;
  std::unique_ptr<ASTOperator> operator_;
  std::unique_ptr<ASTNode> rhs_;
};

class ASTUnaryExpr final : public ASTNode {
public:
  ASTUnaryExpr(SyntaxToken op, std::unique_ptr<ASTNode> operand)
      : operator_{std::make_unique<ASTOperator>(std::move(op))},
        operand_{std::move(operand)} {}
  ASTUnaryExpr(ASTUnaryExpr const &) = delete;
  ASTUnaryExpr &operator=(ASTUnaryExpr const &) = delete;
  ASTUnaryExpr(ASTUnaryExpr &&) noexcept = default;
  ASTUnaryExpr &operator=(ASTUnaryExpr &&) noexcept = default;

  ~ASTUnaryExpr() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::UnaryExpression;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    auto res = std::vector<ASTNode *>{operator_.get(), operand_.get()};
    return res;
  }

  [[nodiscard]] auto GetSource() const -> std::string override {
    return fmt::format("{} {}", operator_->GetSource(), operand_->GetSource());
  }

  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = operator_->GetSourceLocation().first;
    auto const end = operand_->GetSourceLocation().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ASTOperator> operator_;
  std::unique_ptr<ASTNode> operand_;
};

class ASTBracedExpr final : public ASTNode {
public:
  ASTBracedExpr(SyntaxToken open, std::unique_ptr<ASTNode> exp,
                SyntaxToken close)
      : open_{std::move(open)}, expression_{std::move(exp)}, close_{std::move(
                                                                 close)} {}
  ASTBracedExpr(ASTBracedExpr const &) = delete;
  ASTBracedExpr &operator=(ASTBracedExpr const &) = delete;
  ASTBracedExpr(ASTBracedExpr &&) noexcept = default;
  ASTBracedExpr &operator=(ASTBracedExpr &&) noexcept = default;

  ~ASTBracedExpr() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::BracedExpression;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    return std::vector<ASTNode *>{expression_.get()};
  }

  [[nodiscard]] auto GetSource() const -> std::string override {
    return fmt::format("({})", expression_->GetSource());
  }

  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = open_.Position;
    auto const end = close_.Position + 1;
    return std::make_pair(start, end);
  }

private:
  SyntaxToken open_;
  std::unique_ptr<ASTNode> expression_;
  SyntaxToken close_;
};

class ASTUtils {
public:
  static auto PrettyPrint(std::ostream &out, ASTNode const &node,
                          std::string indent = "", bool isLast = true) -> void {
    auto const marker = isLast ? std::string("└── ") : std::string("├── ");

    auto location = node.GetSourceLocation();
    out << fmt::format("{0}{1}<{2}[{3}, {4}) \"{5}\">\n", indent, marker,
                       node.GetType(), location.first, location.second,
                       node.GetSource());

    indent += isLast ? std::string("    ") : std::string("│   ");

    if (auto const children = node.GetChildren(); not children.empty()) {
      std::for_each(children.begin(), children.end() - 1,
                    [&](auto const *const child) {
                      PrettyPrint(out, *child, indent, false);
                    });

      PrettyPrint(out, *children.back(), indent, true);
    }
  }
};
#endif // TCC_PLAYGROUND_AST_HPP