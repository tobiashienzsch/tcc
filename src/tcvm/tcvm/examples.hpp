/**
 * @file examples.hpp
 * @copyright Copyright 2019-2020 Tobias Hienzsch. MIT license.
 */

#pragma once

#include "tcsl/tcsl.hpp"
#include "tcvm/vm/vm.hpp"

namespace tcvm
{

auto createCompiledProgram() -> tcc::BinaryProgram;
auto createAdditionProgram(int64_t arg) -> tcc::BinaryProgram;
auto createFactorialProgram(int64_t argument) -> tcc::BinaryProgram;
auto createFibonacciProgram(int64_t arg) -> tcc::BinaryProgram;
auto createMultipleArgumentsProgram(int64_t firstArg, int64_t secondArg) -> tcc::BinaryProgram;
auto createMultipleFunctionsProgram(int64_t arg) -> tcc::BinaryProgram;

}  // namespace tcvm