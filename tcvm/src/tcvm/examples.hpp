/**
 * @file examples.hpp
 * @copyright Copyright 2019 Tobias Hienzsch. MIT license.
 */

#pragma once

#include "tcc/core/core.hpp"

#include "tcvm/vm/vm.hpp"

namespace tcvm
{

auto CreateCompiledAssembly() -> std::vector<tcc::Integer>;
auto CreateAdditionAssembly(tcc::Integer arg) -> std::vector<tcc::Integer>;
auto CreateFactorialAssembly(tcc::Integer argument) -> std::vector<tcc::Integer>;
auto CreateFibonacciAssembly(tcc::Integer arg) -> std::vector<tcc::Integer>;
auto CreateMultipleArgumentsAssembly(tcc::Integer firstArg, tcc::Integer secondArg) -> std::vector<tcc::Integer>;
auto CreateMultipleFunctionsAssembly(tcc::Integer arg) -> std::vector<tcc::Integer>;

}  // namespace tcvm