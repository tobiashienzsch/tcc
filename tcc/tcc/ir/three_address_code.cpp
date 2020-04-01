#include "tcc/ir/three_address_code.hpp"

#include <sstream>

namespace tcc {
auto operator<<(std::ostream& out, ThreeAddressCode const& data) -> std::ostream& {
  auto const formatArgument = [](auto const& argument) {
    auto result = std::string{};
    std::visit(tcc::overloaded{
                   [&result](int const arg) { result = fmt::format("{}", arg); },
                   [&result](std::string const& arg) { result = fmt::format("%{}", arg); },
               },
               argument);
    return result;
  };

  auto const firstStr = formatArgument(data.first);
  auto const secondStr = data.second.has_value() ? formatArgument(data.second.value()) : "";

  auto opCodeStr = std::stringstream{};
  opCodeStr << static_cast<tcc::byte_code>(data.type);

  return out << fmt::format("{0}\t:=\t{1}\t{2}\t{3}", data.destination, opCodeStr.str(), firstStr, secondStr);
}
}  // namespace tcc