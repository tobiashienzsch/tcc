/**
 * @file examples.hpp
 * @copyright Copyright 2019 Tobias Hienzsch. All rights reserved.
 */

#pragma once

#include "tcc/core/core.hpp"

#include "tcvm/vm/vm.hpp"

namespace tcvm
{
auto const CreateAdditionAssembly = [](tcc::Integer const arg) {
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

auto const CreateFactorialAssembly = [](tcc::Integer argument) {
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

auto const CreateFibonacciAssembly = [](tcc::Integer const arg) {
    using tcc::ByteCode;
    return std::vector<tcc::Integer> {
        // .def fib: args=1, locals=0
        // if (x < 2) return x;
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 2,  // 2
        ByteCode::ILT,        // 4
        ByteCode::BRF, 10,    // 5
        ByteCode::LOAD, -3,   // 7
        ByteCode::RET,        // 9

        // return fib(x - 1) + fib(x - 2)
        ByteCode::LOAD, -3,    // 10
        ByteCode::ICONST, 1,   // 12
        ByteCode::ISUB,        // 14
        ByteCode::CALL, 0, 1,  // 15 <-- fib(x-1)
        ByteCode::LOAD, -3,    // 18
        ByteCode::ICONST, 2,   // 20
        ByteCode::ISUB,        // 22
        ByteCode::CALL, 0, 1,  // 23 <-- fib(x-2)
        ByteCode::IADD,        // 26
        ByteCode::RET,         // 27

        // .def main: args=0, locals=0
        // return fib(arg);
        ByteCode::ICONST, arg,  // 28 <-- MAIN
        ByteCode::CALL, 0, 1,   // 30 <-- fib(arg)
        ByteCode::EXIT,         // 33
    };
};

auto const CreateMultipleArgumentsAssembly = [](tcc::Integer firstArg, tcc::Integer secondArg) {
    using tcc::ByteCode;
    return std::vector<tcc::Integer> {
        // .def sub: args=2, locals=0
        // return x + y;
        ByteCode::LOAD, -4,  // 0 <-- load x
        ByteCode::LOAD, -3,  // 2 <-- load y
        ByteCode::ISUB,      // 4
        ByteCode::RET,       // 5

        // .def main: args=0, locals=0
        // x = sub(10, 2);
        // y = sub(x, 5);
        // exit x;
        ByteCode::ICONST, firstArg,   // 6 <-- MAIN
        ByteCode::ICONST, secondArg,  // 8
        ByteCode::CALL, 0, 2,         // 10 <-- sub(10, 2)
        ByteCode::ICONST, 5,          // 13
        ByteCode::CALL, 0, 2,         // 13 <-- sub(x, 5)
        ByteCode::EXIT,               // 13
    };
};

auto const CreateMultipleFunctionsAssembly = [](tcc::Integer arg) {
    using tcc::ByteCode;
    return std::vector<tcc::Integer> {
        // .def func1: args=1, locals=0
        // return x * 3;
        ByteCode::LOAD, -3,   // 0
        ByteCode::ICONST, 3,  // 2
        ByteCode::IMUL,       // 4
        ByteCode::RET,        // 5

        // .def func2: args=1, locals=0
        // return x + 10;
        ByteCode::LOAD, -3,    // 6
        ByteCode::ICONST, 10,  // 8
        ByteCode::IADD,        // 10
        ByteCode::RET,         // 11

        // .def main: args=0, locals=0
        // x = func1(2);
        // y = func2(x);
        // exit y;
        ByteCode::ICONST, arg,  // 12 <-- MAIN
        ByteCode::CALL, 0, 1,   // 14 <-- func1(2)
        ByteCode::CALL, 6, 1,   // 17 <-- func2(func1(2))
        ByteCode::EXIT,         // 21
    };
};

}  // namespace tcvm