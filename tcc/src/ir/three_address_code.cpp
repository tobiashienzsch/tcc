#include "ir/three_address_code.hpp"

#include <sstream>

namespace tcc
{
auto operator<<(std::ostream& out, ThreeAddressCode const& data) -> std::ostream&
{
    auto firstStr = std::string {};
    std::visit(tcc::overloaded {
                   [&firstStr](int arg) { firstStr = fmt::format("{}", arg); },
                   [&firstStr](const std::string& arg) { firstStr = fmt::format("%{}", arg); },
               },
               data.first);

    auto secondStr = std::string {};
    if (data.second.has_value())
    {
        auto const& sec = data.second.value();
        std::visit(tcc::overloaded {
                       [&secondStr](int arg) { secondStr = fmt::format("{}", arg); },
                       [&secondStr](const std::string& arg) { secondStr = fmt::format("%{}", arg); },
                   },
                   sec);
    }

    std::stringstream opCodeStr;
    opCodeStr << static_cast<tcc::byte_code>(data.type);
    auto const str = fmt::format("{0}\t{4}\t:=\t{1}\t{2}\t{3}", data.destination, firstStr, opCodeStr.str(), secondStr,
                                 data.isTemporary);

    return out << str;
}
}  // namespace tcc