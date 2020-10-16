#ifndef TCC_PLAYGROUND_AST_HPP
#define TCC_PLAYGROUND_AST_HPP

#include "token.hpp"

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

enum class ASTNodeType {
  Operand,
  ConstantExpression,
  BinaryExpression,
};

std::ostream &operator<<(std::ostream &out, ASTNodeType const type);

class ASTNode {
public:
  virtual ~ASTNode() = default;

  [[nodiscard]] virtual ASTNodeType GetType() const = 0;
  [[nodiscard]] virtual std::vector<ASTNode *> GetChildren() const = 0;
  [[nodiscard]] virtual std::pair<std::size_t, std::size_t>
  GetSourceLocation() const = 0;
};

class ASTOperand final : public ASTNode {
public:
  ASTOperand(SyntaxToken token) : token_{std::move(token)} {}
  ASTOperand(ASTOperand const &) = delete;
  ASTOperand &operator=(ASTOperand const &) = delete;
  ASTOperand(ASTOperand &&) noexcept = default;
  ASTOperand &operator=(ASTOperand &&) noexcept = default;
  ~ASTOperand() override = default;

  [[nodiscard]] ASTNodeType GetType() const override {
    return ASTNodeType::Operand;
  }

  [[nodiscard]] std::vector<ASTNode *> GetChildren() const override {
    return {};
  }

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  GetSourceLocation() const override {
    return std::make_pair(token_.Position,
                          token_.Position + token_.Text.size());
  }

  [[nodiscard]] SyntaxToken Token() const noexcept { return token_; }

private:
  SyntaxToken token_;
};

class ASTConstantExpr final : public ASTNode {
public:
  ASTConstantExpr(SyntaxToken token) : token_{std::move(token)} {}
  ASTConstantExpr(ASTConstantExpr const &) = delete;
  ASTConstantExpr &operator=(ASTConstantExpr const &) = delete;
  ASTConstantExpr(ASTConstantExpr &&) noexcept = default;
  ASTConstantExpr &operator=(ASTConstantExpr &&) noexcept = default;
  ~ASTConstantExpr() override = default;

  [[nodiscard]] ASTNodeType GetType() const override {
    return ASTNodeType::ConstantExpression;
  }

  [[nodiscard]] std::vector<ASTNode *> GetChildren() const override {
    return {};
  }

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  GetSourceLocation() const override {
    return std::make_pair(token_.Position,
                          token_.Position + token_.Text.size());
  }

  [[nodiscard]] SyntaxToken Token() const noexcept { return token_; }

private:
  SyntaxToken token_;
};

class ASTBinaryExpr final : public ASTNode {
public:
  ASTBinaryExpr(std::unique_ptr<ASTNode> lhs, SyntaxToken op,
                std::unique_ptr<ASTNode> rhs)
      : lhs_{std::move(lhs)},
        operand_{std::make_unique<ASTOperand>(std::move(op))}, rhs_{std::move(
                                                                   rhs)} {}
  ASTBinaryExpr(ASTBinaryExpr const &) = delete;
  ASTBinaryExpr &operator=(ASTBinaryExpr const &) = delete;
  ASTBinaryExpr(ASTBinaryExpr &&) noexcept = default;
  ASTBinaryExpr &operator=(ASTBinaryExpr &&) noexcept = default;

  ~ASTBinaryExpr() override = default;

  [[nodiscard]] ASTNodeType GetType() const override {
    return ASTNodeType::BinaryExpression;
  }

  [[nodiscard]] std::vector<ASTNode *> GetChildren() const override {
    auto res = std::vector<ASTNode *>{lhs_.get(), operand_.get(), rhs_.get()};
    return res;
  }

  [[nodiscard]] std::pair<std::size_t, std::size_t>
  GetSourceLocation() const override {
    auto const start = lhs_->GetSourceLocation().first;
    auto const end = rhs_->GetSourceLocation().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ASTNode> lhs_;
  std::unique_ptr<ASTOperand> operand_;
  std::unique_ptr<ASTNode> rhs_;
};

class ASTUtils {
public:
  static auto PrettyPrint(std::ostream &out, ASTNode const &node,
                          std::string indent = "", bool isLast = true) -> void {
    auto const marker = isLast ? std::string("└── ") : std::string("├── ");

    out << indent;
    out << marker;
    out << node.GetType() << "[" << node.GetSourceLocation().first << ", "
        << node.GetSourceLocation().second << ") ";

    auto const *constant = dynamic_cast<ASTConstantExpr const *>(&node);
    if (constant != nullptr) {
      out << constant->Token().Text;
    }

    auto const *operand = dynamic_cast<ASTOperand const *>(&node);
    if (operand != nullptr) {
      out << operand->Token().Type;
    }

    out << '\n';

    indent += isLast ? std::string("    ") : std::string("│   ");
    auto const children = node.GetChildren();
    if (not children.empty()) {
      std::for_each(
          children.begin(), children.end() - 1,
          [&](auto const &child) { PrettyPrint(out, *child, indent, false); });
      PrettyPrint(out, *children.back(), indent, true);
    }
  }
};
#endif // TCC_PLAYGROUND_AST_HPP
