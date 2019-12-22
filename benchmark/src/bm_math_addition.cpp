#include <benchmark/benchmark.h>

#include "tcvm/vm/vm.hpp"

namespace
{
constexpr auto addition(int64_t x, int64_t y) -> int64_t
{
    auto const l1 = x + y;
    auto const l2 = y + 10;
    return l1 + l2;
};

static_assert(addition(10, 10) == 40);
static_assert(addition(7, 10) == 37);

auto const createAdditionAssembly = [](int64_t const arg) {
    using tcc::ByteCode;
    return std::vector<int64_t>{
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
}  // namespace

static void BM_CppAddition(benchmark::State& state)
{
    auto const arg = state.range(0);
    for (auto _ : state)
    {
        auto result = addition(arg, 10);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CppAddition)->Arg(7)->Arg(10);

static void BM_StackMachineAddition(benchmark::State& state)
{

    auto const assembly = createAdditionAssembly(state.range(0));
    auto vm             = tcc::VirtualMachine(assembly, 18, 0, 200, false);

    for (auto _ : state)
    {
        vm.Reset(18);
        auto const exitCode = vm.Cpu();
        benchmark::DoNotOptimize(exitCode);
    }
}
BENCHMARK(BM_StackMachineAddition)->Arg(7)->Arg(10);