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

enum class ast_node_type {
  op,
  constant,
  binary_expression,
  unary_expression,
  braced_expression,
};

auto operator<<(std::ostream &out, ast_node_type type) -> std::ostream &;

// NOLINTNEXTLINE(hicpp-special-member-functions)
class ast_node {
public:
  virtual ~ast_node() = default;

  [[nodiscard]] virtual auto get_type() const -> ast_node_type = 0;
  [[nodiscard]] virtual auto get_source() const -> std::string = 0;
  [[nodiscard]] virtual auto get_children() const
      -> std::vector<ast_node *> = 0;
  [[nodiscard]] virtual auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> = 0;
};

class ast_operator final : public ast_node {
public:
  explicit ast_operator(syntax_token token) : token_{std::move(token)} {}
  ast_operator(ast_operator const &) = delete;
  auto operator=(ast_operator const &) -> ast_operator & = delete;
  ast_operator(ast_operator &&) noexcept = default;
  auto operator=(ast_operator &&) noexcept -> ast_operator & = default;
  ~ast_operator() override = default;

  [[nodiscard]] auto get_type() const -> ast_node_type override {
    return ast_node_type::op;
  }

  [[nodiscard]] auto get_children() const -> std::vector<ast_node *> override {
    return {};
  }

  [[nodiscard]] auto get_source() const -> std::string override {
    return token_.text;
  }

  [[nodiscard]] auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> override {
    return std::make_pair(token_.position,
                          token_.position + token_.text.size());
  }

  [[nodiscard]] auto token() const noexcept -> syntax_token { return token_; }

private:
  syntax_token token_;
};

class ast_constant final : public ast_node {
public:
  explicit ast_constant(syntax_token token) : token_{std::move(token)} {}
  ast_constant(ast_constant const &) = delete;
  auto operator=(ast_constant const &) -> ast_constant & = delete;
  ast_constant(ast_constant &&) noexcept = default;
  auto operator=(ast_constant &&) noexcept -> ast_constant & = default;
  ~ast_constant() override = default;

  [[nodiscard]] auto get_type() const -> ast_node_type override {
    return ast_node_type::constant;
  }

  [[nodiscard]] auto get_children() const -> std::vector<ast_node *> override {
    return {};
  }
  [[nodiscard]] auto get_source() const -> std::string override {
    return token_.text;
  }
  [[nodiscard]] auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> override {
    return std::make_pair(token_.position,
                          token_.position + token_.text.size());
  }

  [[nodiscard]] auto token() const noexcept -> syntax_token { return token_; }

private:
  syntax_token token_;
};

class ast_binary_expr final : public ast_node {
public:
  ast_binary_expr(std::unique_ptr<ast_node> lhs, syntax_token const &op,
                  std::unique_ptr<ast_node> rhs)
      : lhs_{std::move(lhs)}, operator_{std::make_unique<ast_operator>(op)},
        rhs_{std::move(rhs)} {}
  ast_binary_expr(ast_binary_expr const &) = delete;
  auto operator=(ast_binary_expr const &) -> ast_binary_expr & = delete;
  ast_binary_expr(ast_binary_expr &&) noexcept = default;
  auto operator=(ast_binary_expr &&) noexcept -> ast_binary_expr & = default;

  ~ast_binary_expr() override = default;

  [[nodiscard]] auto get_type() const -> ast_node_type override {
    return ast_node_type::binary_expression;
  }

  [[nodiscard]] auto get_children() const -> std::vector<ast_node *> override {
    auto res = std::vector<ast_node *>{lhs_.get(), operator_.get(), rhs_.get()};
    return res;
  }

  [[nodiscard]] auto get_source() const -> std::string override {
    return fmt::format("{} {} {}", lhs_->get_source(), operator_->get_source(),
                       rhs_->get_source());
  }

  [[nodiscard]] auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = lhs_->get_source_location().first;
    auto const end = rhs_->get_source_location().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ast_node> lhs_;
  std::unique_ptr<ast_operator> operator_;
  std::unique_ptr<ast_node> rhs_;
};

class ast_unary_expr final : public ast_node {
public:
  ast_unary_expr(syntax_token const &op, std::unique_ptr<ast_node> operand)
      : operator_{std::make_unique<ast_operator>(op)}, operand_{std::move(
                                                           operand)} {}
  ast_unary_expr(ast_unary_expr const &) = delete;
  auto operator=(ast_unary_expr const &) -> ast_unary_expr & = delete;
  ast_unary_expr(ast_unary_expr &&) noexcept = default;
  auto operator=(ast_unary_expr &&) noexcept -> ast_unary_expr & = default;

  ~ast_unary_expr() override = default;

  [[nodiscard]] auto get_type() const -> ast_node_type override {
    return ast_node_type::unary_expression;
  }

  [[nodiscard]] auto get_children() const -> std::vector<ast_node *> override {
    auto res = std::vector<ast_node *>{operator_.get(), operand_.get()};
    return res;
  }

  [[nodiscard]] auto get_source() const -> std::string override {
    return fmt::format("{} {}", operator_->get_source(),
                       operand_->get_source());
  }

  [[nodiscard]] auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = operator_->get_source_location().first;
    auto const end = operand_->get_source_location().second;
    return std::make_pair(start, end);
  }

private:
  std::unique_ptr<ast_operator> operator_;
  std::unique_ptr<ast_node> operand_;
};

class ast_braced_expr final : public ast_node {
public:
  ast_braced_expr(syntax_token open, std::unique_ptr<ast_node> exp,
                  syntax_token close)
      : open_{std::move(open)}, expression_{std::move(exp)}, close_{std::move(
                                                                 close)} {}
  ast_braced_expr(ast_braced_expr const &) = delete;
  auto operator=(ast_braced_expr const &) -> ast_braced_expr & = delete;
  ast_braced_expr(ast_braced_expr &&) noexcept = default;
  auto operator=(ast_braced_expr &&) noexcept -> ast_braced_expr & = default;

  ~ast_braced_expr() override = default;

  [[nodiscard]] auto get_type() const -> ast_node_type override {
    return ast_node_type::braced_expression;
  }

  [[nodiscard]] auto get_children() const -> std::vector<ast_node *> override {
    return std::vector<ast_node *>{expression_.get()};
  }

  [[nodiscard]] auto get_source() const -> std::string override {
    return fmt::format("({})", expression_->get_source());
  }

  [[nodiscard]] auto get_source_location() const
      -> std::pair<std::size_t, std::size_t> override {
    auto const start = open_.position;
    auto const end = close_.position + 1;
    return std::make_pair(start, end);
  }

private:
  syntax_token open_;
  std::unique_ptr<ast_node> expression_;
  syntax_token close_;
};

class ast_utils {
public:
  static auto pretty_print(std::ostream &out, ast_node const &node,
                           std::string indent = "", bool is_last = true)
      -> void {
    auto const marker = is_last ? std::string("└── ") : std::string("├── ");

    auto location = node.get_source_location();
    out << fmt::format("{0}{1}<{2}[{3}, {4})>\n", indent, marker,
                       node.get_type(), location.first, location.second);

    indent += is_last ? std::string("    ") : std::string("│   ");

    if (auto const children = node.get_children(); not children.empty()) {
      std::for_each(children.begin(), children.end() - 1,
                    [&](auto const *const child) {
                      pretty_print(out, *child, indent, false);
                    });

      pretty_print(out, *children.back(), indent, true);
    }
  }
};
#endif // TCC_PLAYGROUND_AST_HPP
