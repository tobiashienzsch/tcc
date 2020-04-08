/**
 * @file byte_code.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include <array>
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
    EXIT   = 18,
    HALT   = 19,
};

std::ostream& operator<<(std::ostream& out, ByteCode byteCode);

struct Instruction
{
    constexpr explicit Instruction(std::string_view n) : name(n), numberOfOperands(0) { }
    constexpr explicit Instruction(std::string_view n, int8_t numOperands) : name(n), numberOfOperands(numOperands) { }
    std::string_view name = {};
    int8_t numberOfOperands;
};

constexpr auto Instructions = std::array {
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