#include <benchmark/benchmark.h>

#include "tcvm/vm/vm.hpp"

namespace
{
constexpr auto fibonacci(int64_t x) -> int64_t
{
    if (x < 2) return x;
    return fibonacci(x - 1) + fibonacci(x - 2);
}

static_assert(fibonacci(1) == 1);
static_assert(fibonacci(2) == 1);
static_assert(fibonacci(5) == 5);
static_assert(fibonacci(12) == 144);

auto const createFibonacciAssembly = [](int64_t const arg) {
    using tcc::ByteCode;
    return std::vector<int64_t>{
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
}  // namespace

static void BM_CppFibonacci(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto result = fibonacci(state.range(0));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CppFibonacci)->Arg(3)->Arg(12)->Arg(15);

static void BM_StackMachineFibonacci(benchmark::State& state)
{

    auto const assembly = createFibonacciAssembly(state.range(0));
    auto vm             = tcc::VirtualMachine(assembly, 28, 0, 200, false);

    for (auto _ : state)
    {
        vm.Reset(28);
        auto const exitCode = vm.Cpu();
        benchmark::DoNotOptimize(exitCode);
    }
}
BENCHMARK(BM_StackMachineFibonacci)->Arg(3)->Arg(12)->Arg(15);
