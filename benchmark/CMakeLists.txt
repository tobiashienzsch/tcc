project(tcc-benchmark)

add_executable(benchmark_math 
    src/bm_factorial.cpp
    src/bm_fibonacci.cpp
)
target_link_libraries(benchmark_math PRIVATE 
    tcc::Library
    benchmark
)
