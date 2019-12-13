#pragma once
#include <cstdint>

#include <iostream>
#include <string_view>

namespace tcc
{
enum ByteCode : int64_t
{
    IADD   = 1,
    ISUB   = 2,
    IMUL   = 3,
    ILT    = 4,
    IEQ    = 5,
    BR     = 6,
    BRT    = 7,
    BRF    = 8,
    ICONST = 9,
    LOAD   = 10,
    GLOAD  = 11,
    STORE  = 12,
    GSTORE = 13,
    PRINT  = 14,
    POP    = 15,
    CALL   = 16,
    RET    = 17,
    HALT   = 18,
};

std::ostream& operator<<(std::ostream& out, ByteCode byteCode)
{
    switch (byteCode)
    {
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
        case ByteCode::HALT: return out << "HALT";
    }
    return out;
}

struct Instruction
{
    std::string_view name;
    int8_t numberOfOperands;

    constexpr explicit Instruction(std::string_view _name) : name(_name), numberOfOperands(0) {}
    constexpr explicit Instruction(std::string_view _name, int8_t numOperands)
        : name(_name), numberOfOperands(numOperands)
    {
    }
};

constexpr Instruction Instructions[] = {
    Instruction{"invalid"},    //
    Instruction{"iadd"},       //
    Instruction{"isub"},       //
    Instruction{"imul"},       //
    Instruction{"ilt"},        //
    Instruction{"ieq"},        //
    Instruction{"br", 1},      //
    Instruction{"brt", 1},     //
    Instruction{"brf", 1},     //
    Instruction{"iconst", 1},  //
    Instruction{"load", 1},    //
    Instruction{"gload", 1},   //
    Instruction{"store", 1},   //
    Instruction{"gstore", 1},  //
    Instruction{"print"},      //
    Instruction{"pop"},        //
    Instruction{"call", 2},    //
    Instruction{"ret"},        //
    Instruction{"halt"},       //
};

}  // namespace tcc