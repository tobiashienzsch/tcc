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
    NOOP = 0,
    IADD = 1,
    ISUB,
    IMUL,
    ILT,
    IEQ,
    BR,
    BRT,
    BRF,
    ICONST,
    LOAD,
    GLOAD,
    STORE,
    GSTORE,
    PRINT,
    POP,
    CALL,
    RET,
    EXIT,
    HALT,
    NUM_OPCODES,
};

std::ostream& operator<<(std::ostream& out, ByteCode byteCode);

struct Instruction
{
    constexpr explicit Instruction(std::string_view n) : name(n), numberOfOperands(0) { }
    constexpr explicit Instruction(std::string_view n, int8_t numOperands)
        : name(n)
        , numberOfOperands(numOperands)
    {
    }
    std::string_view name = {};
    int8_t numberOfOperands;
};

constexpr auto Instructions = std::array {
    Instruction {"noop"},       //
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