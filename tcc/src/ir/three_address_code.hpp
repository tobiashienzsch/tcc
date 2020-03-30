#pragma once

#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "tcc/core.hpp"
#include "vm.hpp"

namespace tcc {
struct ThreeAddressCode {
  using Argument = std::variant<int, std::string>;
  using OptionalArgument = std::optional<Argument>;

  tcc::byte_code type;
  std::string destination;
  Argument first;
  OptionalArgument second;
  bool isTemporary{true};
};

auto operator<<(std::ostream& out, ThreeAddressCode const& data)
    -> std::ostream&;

using StatementList = std::vector<ThreeAddressCode>;

struct StatementScope {
  std::string name = "";
  StatementList statements = {};
  StatementScope* parent = nullptr;
  std::vector<StatementScope*> children = {nullptr};
  std::map<std::string, int> variables = {};
};

inline auto operator<<(std::ostream& out, StatementScope const& data)
    -> std::ostream& {
  return out << data.name;
}

}  // namespace tcc