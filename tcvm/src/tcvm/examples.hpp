/**
 * @file examples.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include "tcc/core.hpp"
#include "tcvm/vm/vm.hpp"

namespace tcvm {

auto CreateCompiledProgram() -> tcc::BinaryProgram;
auto CreateAdditionProgram(int64_t arg) -> tcc::BinaryProgram;
auto CreateFactorialProgram(int64_t argument) -> tcc::BinaryProgram;
auto CreateFibonacciProgram(int64_t arg) -> tcc::BinaryProgram;
auto CreateMultipleArgumentsProgram(int64_t firstArg, int64_t secondArg)
    -> tcc::BinaryProgram;
auto CreateMultipleFunctionsProgram(int64_t arg) -> tcc::BinaryProgram;

}  // namespace tcvm