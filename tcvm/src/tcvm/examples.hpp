#pragma once

#include "tcc/core/core.hpp"

#include "tcc/vm/vm.hpp"

namespace tcvm
{
auto const createAdditionAssembly = [](tcc::Integer const arg) {
    using tcc::ByteCode;
    return std::vector<tcc::Integer> {
        // .def addition: args=2, locals=1
        // l1 = x + y
        ByteCode::ICONST, 0,  // 0 local #1
        ByteCode::LOAD, -4,   // 2
        ByteCode::LOAD, -3,   // 4
        ByteCode::IADD,       // 6
        ByteCode::STORE, 1,   // 7

        // l2 = y + 10
        ByteCode::LOAD, -3,    // 9
        ByteCode::ICONST, 10,  // 11
        ByteCode::IADD,        // 13

        // l1 + l2
        ByteCode::LOAD, 1,  // 14
        ByteCode::IADD,     // 16
        ByteCode::RET,      // 17

        // .def main: args=0, locals=0
        // foo(10, 10)
        ByteCode::ICONST, arg,  // 18 <-- MAIN
        ByteCode::ICONST, 10,   // 20
        ByteCode::CALL, 0, 1,   // 22
        ByteCode::EXIT,         // 25
    };
};

auto const createFactorialAssembly = [](tcc::Integer argument) {
    using tcc::ByteCode;
    return std::vector<tcc::Integer> {
        // .def fact: args=1, locals=0
        // if n < 2 return 1
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 2,  // 2
        ByteCode::ILT,        // 4
        ByteCode::BRF, 10,    // 5
        ByteCode::ICONST, 1,  // 7
        ByteCode::RET,        // 9

        // return n * fact(n-1)
        ByteCode::LOAD, -3,    // 10
        ByteCode::LOAD, -3,    // 12
        ByteCode::ICONST, 1,   // 14
        ByteCode::ISUB,        // 16
        ByteCode::CALL, 0, 1,  // 17
        ByteCode::IMUL,        // 20
        ByteCode::RET,         // 21

        // .def main: args=0, locals=0
        ByteCode::ICONST, argument,  // 22 <-- MAIN
        ByteCode::CALL, 0, 1,        // 24
        ByteCode::EXIT,              // 27
    };
};
}  // namespace tcvm