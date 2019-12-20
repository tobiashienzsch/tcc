#pragma once
#include <cstdint>

#include <iostream>
#include <string_view>

#include "tcc/core/core.hpp"

namespace tcc
{
enum ByteCode : Integer
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
    EXIT   = 18,
    HALT   = 19,
};

std::ostream& operator<<(std::ostream& out, ByteCode byteCode);

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
    Instruction {"invalid"},    //
    Instruction {"iadd"},       //
    Instruction {"isub"},       //
    Instruction {"imul"},       //
    Instruction {"ilt"},        //
    Instruction {"ieq"},        //
    Instruction {"br", 1},      //
    Instruction {"brt", 1},     //
    Instruction {"brf", 1},     //
    Instruction {"iconst", 1},  //
    Instruction {"load", 1},    //
    Instruction {"gload", 1},   //
    Instruction {"store", 1},   //
    Instruction {"gstore", 1},  //
    Instruction {"print"},      //
    Instruction {"pop"},        //
    Instruction {"call", 2},    //
    Instruction {"ret"},        //
    Instruction {"exit"},       //
    Instruction {"halt"},       //
};

}  // namespace tcc