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
  Operand,
  ConstantExpression,
  BinaryExpression,
  BracedExpression,
};

auto operator<<(std::ostream &out, ASTNodeType type) -> std::ostream &;

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

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::Operand;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    return {};
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

class ASTConstantExpr final : public ASTNode {
public:
  ASTConstantExpr(SyntaxToken token) : token_{std::move(token)} {}
  ASTConstantExpr(ASTConstantExpr const &) = delete;
  ASTConstantExpr &operator=(ASTConstantExpr const &) = delete;
  ASTConstantExpr(ASTConstantExpr &&) noexcept = default;
  ASTConstantExpr &operator=(ASTConstantExpr &&) noexcept = default;
  ~ASTConstantExpr() override = default;

  [[nodiscard]] auto GetType() const -> ASTNodeType override {
    return ASTNodeType::ConstantExpression;
  }

  [[nodiscard]] auto GetChildren() const -> std::vector<ASTNode *> override {
    return {};
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
        operand_{std::make_unique<ASTOperand>(std::move(op))}, rhs_{std::move(
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
    auto res = std::vector<ASTNode *>{lhs_.get(), operand_.get(), rhs_.get()};
    return res;
  }

  [[nodiscard]] auto GetSourceLocation() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = lhs_->GetSourceLocation().first;
    auto const end = rhs_->GetSourceLocation().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ASTNode> lhs_;
  std::unique_ptr<ASTOperand> operand_;
  std::unique_ptr<ASTNode> rhs_;
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
    out << fmt::format("{0}{1}<{2}[{3}, {4}) >\n", indent, marker, node.GetType(),
                       location.first, location.second);


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
