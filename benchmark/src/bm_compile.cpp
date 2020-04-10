#include <benchmark/benchmark.h>

#include "tcc/compiler/compiler.hpp"
#include "tcsl/tcsl.hpp"

static void BM_CompileMinimal(benchmark::State& state)
{

    auto source = std::string {R"(
        int main()
        {
            int x = 1;
            int y = 2;
            return x + y;
        }
    )"};

    auto options     = tcc::CompilerOptions {};
    options.Source   = source;
    options.OptLevel = state.range(0);

    for (auto _ : state)
    {

        auto compiler = tcc::Compiler {options};
        benchmark::DoNotOptimize(compiler.Run());
    }
}
BENCHMARK(BM_CompileMinimal)->Arg(0)->Arg(1)->Arg(2);

static void BM_CompileSimpleFunctions(benchmark::State& state)
{

    auto source = std::string {R"(
        int foo(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int main()
        {
            int x = 1 + 4;
            int y = foo(foo(x));
            return x + y;
        }
    )"};

    auto options     = tcc::CompilerOptions {};
    options.Source   = source;
    options.OptLevel = state.range(0);

    for (auto _ : state)
    {

        auto compiler = tcc::Compiler {options};
        benchmark::DoNotOptimize(compiler.Run());
    }
}
BENCHMARK(BM_CompileSimpleFunctions)->Arg(0)->Arg(1)->Arg(2);

static void BM_CompileManyFunctions(benchmark::State& state)
{

    auto source = std::string {R"(
        int foo(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo2(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo3(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo4(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo5(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo6(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int foo7(a)
        {
            int b = 30;
            int q = 10 * 2;
            int p = b - q;
            int c = p * 15;
            return c + a;
        }

        int main()
        {
            int x = 1 + 4;
            int y = foo(foo(x));
            y = foo2(foo2(x));
            y = foo3(foo3(x));
            y = foo4(foo4(x));
            y = foo5(foo5(x));
            return x + y;
        }
    )"};

    auto options     = tcc::CompilerOptions {};
    options.Source   = source;
    options.OptLevel = state.range(0);

    for (auto _ : state)
    {

        auto compiler = tcc::Compiler {options};
        benchmark::DoNotOptimize(compiler.Run());
    }
}
BENCHMARK(BM_CompileManyFunctions)->Arg(0)->Arg(1)->Arg(2);

BENCHMARK_MAIN();
