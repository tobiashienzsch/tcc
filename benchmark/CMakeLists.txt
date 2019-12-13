project(tcc-benchmark)

add_executable(factorial src/bm_factorial.cpp)
target_link_libraries(factorial PRIVATE 
    tcc::Library
    benchmark
)
