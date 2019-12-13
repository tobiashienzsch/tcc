#include <benchmark/benchmark.h>

constexpr auto factorial(int32_t val) -> int32_t;
constexpr auto factorial(int32_t val) -> int32_t
{
    if (val <= 1) return 1;
    return val * factorial(val - 1);
}

static_assert(factorial(3) == 6);

static void BM_CppFactorial(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto result = factorial(7);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CppFactorial);

static void BM_StackMachineFactorial(benchmark::State& state)
{
    std::string x = "hello";
    for (auto _ : state)
    {
        std::string copy(x);
        benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK(BM_StackMachineFactorial);

BENCHMARK_MAIN();
