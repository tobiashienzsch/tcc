/**
 * @file examples.hpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */

#pragma once

#include "tcc/core/binary_format.hpp"
#include "tcc/core/core.hpp"

#include "tcvm/vm/vm.hpp"

namespace tcvm
{

auto CreateCompiledProgram() -> tcc::BinaryProgram;
auto CreateAdditionProgram(tcc::Integer arg) -> tcc::BinaryProgram;
auto CreateFactorialProgram(tcc::Integer argument) -> tcc::BinaryProgram;
auto CreateFibonacciProgram(tcc::Integer arg) -> tcc::BinaryProgram;
auto CreateMultipleArgumentsProgram(tcc::Integer firstArg, tcc::Integer secondArg) -> tcc::BinaryProgram;
auto CreateMultipleFunctionsProgram(tcc::Integer arg) -> tcc::BinaryProgram;

}  // namespace tcvm