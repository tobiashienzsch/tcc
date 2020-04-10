project(tcc-benchmark)

add_executable(benchmark_math 
    src/bm_math_factorial.cpp
    src/bm_math_fibonacci.cpp
    src/bm_math_addition.cpp
)
target_link_libraries(benchmark_math 
PRIVATE 
    tcc::CompilerOptions
    tcc::tcc
    tcc::tcvm
    benchmark
)

add_executable(benchmark_compile 
    src/bm_compile.cpp
)
target_link_libraries(benchmark_compile 
PRIVATE 
    tcc::CompilerOptions
    tcc::tcc
    tcc::tcvm
    benchmark
)