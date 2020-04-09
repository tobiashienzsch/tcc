/**
 * @file byte_code.cpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#include "tsl/byte_code.hpp"

#include "fmt/format.h"

namespace tcc
{

auto operator<<(std::ostream& out, ByteCode byteCode) -> std::ostream&
{
    switch (byteCode)
    {
        case ByteCode::NOOP: return out << "NOOP";
        case ByteCode::IADD: return out << "IADD";
        case ByteCode::ISUB: return out << "ISUB";
        case ByteCode::IMUL: return out << "IMUL";
        case ByteCode::ILT: return out << "ILT";
        case ByteCode::IEQ: return out << "IEQ";
        case ByteCode::BR: return out << "BR";
        case ByteCode::BRT: return out << "BRT";
        case ByteCode::BRF: return out << "BRF";
        case ByteCode::ICONST: return out << "ICONST";
        case ByteCode::LOAD: return out << "LOAD";
        case ByteCode::GLOAD: return out << "GLOAD";
        case ByteCode::STORE: return out << "STORE";
        case ByteCode::GSTORE: return out << "GSTORE";
        case ByteCode::PRINT: return out << "PRINT";
        case ByteCode::POP: return out << "POP";
        case ByteCode::CALL: return out << "CALL";
        case ByteCode::RET: return out << "RET";
        case ByteCode::EXIT: return out << "EXIT";
        case ByteCode::HALT: return out << "HALT";
        case ByteCode::NUM_OPCODES:
            return out << fmt::format("{}", static_cast<int64_t>(byteCode));
    }
    return out;
}
}  // namespace tcc