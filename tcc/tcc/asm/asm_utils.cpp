#include "tcc/asm/asm_utils.hpp"

#include "tcsl/tcsl.hpp"

#include <ostream>
#include <set>

namespace tcc
{
auto ASMUtils::PrettyPrint(std::ostream& out, Assembly const& assembly) -> void
{
    auto const& code = assembly.first;
    auto ops         = std::set<tcc::ByteCode::Type> {};
    auto str         = std::string {};

    for (auto i = 0UL; i < code.size();)
    {
        auto const op = static_cast<tcc::ByteCode::Type>(code.at(i));
        ops.emplace(op);
        str.append(fmt::format("\t{0}", op));

        switch (op)
        {
            case ByteCode::HALT: break;
            case ByteCode::RET: break;
            case ByteCode::EXIT: break;
            case ByteCode::IADD: break;
            case ByteCode::IMUL: break;
            case ByteCode::ISUB: break;

            case ByteCode::LOAD:
            {
                str.append(fmt::format(",\t{}", code.at(++i)));
                break;
            }
            case ByteCode::STORE:
            {
                str.append(fmt::format(",\t{}", code.at(++i)));
                break;
            }
            case ByteCode::ICONST:
            {
                str.append(fmt::format(",\t{}", code.at(++i)));
                break;
            }
            case ByteCode::CALL:
            {
                str.append(fmt::format(",\t{}", code.at(++i)));
                str.append(fmt::format(",\t{}", code.at(++i)));
                break;
            }
            default:
            {
                TCC_ASSERT(false, fmt::format("Unknown operation {}", op));
                std::exit(EXIT_FAILURE);
            }
        }

        i++;
        str.append(fmt::format("\n"));
    }

    out << fmt::format("\n// asm: length={0} ops={1}/{2}\n{3}\n", code.size(), ops.size(),
                       static_cast<int>(ByteCode::NUM_OPCODES), str);
}

}  // namespace tcc