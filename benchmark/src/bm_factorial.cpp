#include <benchmark/benchmark.h>

#include "tcc/vm/vm.hpp"

constexpr auto factorial(int32_t val) -> int32_t
{
    if (val <= 1) return 1;
    return val * factorial(val - 1);
}

static_assert(factorial(1) == 1);
static_assert(factorial(2) == 2);
static_assert(factorial(3) == 6);
static_assert(factorial(4) == 24);

constexpr static auto ARGUMENT = 10;

static void BM_CppFactorial(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto result = factorial(ARGUMENT);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CppFactorial);

static void BM_StackMachineFactorial(benchmark::State& state)
{
    using tcc::ByteCode;

    auto const factorial = std::vector<tcc::Integer>{
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
        ByteCode::ICONST, ARGUMENT,  // 22 <-- MAIN
        ByteCode::CALL, 0, 1,        // 24
        ByteCode::EXIT,              // 27
    };

    for (auto _ : state)
    {
        auto vm             = tcc::VirtualMachine(factorial, 22, 0, 200, false);
        auto const exitCode = vm.Cpu();
        benchmark::DoNotOptimize(exitCode);
    }
}
BENCHMARK(BM_StackMachineFactorial);

BENCHMARK_MAIN();
