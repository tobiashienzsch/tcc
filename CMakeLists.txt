cmake_minimum_required(VERSION 3.12)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Options
project(TobanteCrappyCompiler CXX C)

option(TCC_BUILD_TESTS      "Build the unit tests with Catch2"  ON)
option(TCC_BUILD_BENCHMARK  "Build benchmark tests"             ON)
option(TCC_BUILD_COVERAGE   "Build with code coverage"          OFF)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(tccCodeCoverage)
include(tccCompilerOptions)

add_subdirectory(lib)
add_subdirectory(src)

# Testing
if(TCC_BUILD_TESTS)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
    enable_testing()
    add_subdirectory(3rd_party/Catch2 EXCLUDE_FROM_ALL)
    include(CTest)
    include(Catch)
    add_subdirectory(test)
endif()

# Benchmark
if(TCC_BUILD_BENCHMARK)
    set(ALLOW_DOWNLOADING_GOOGLETEST FALSE)
    add_subdirectory(3rd_party/google-benchmark EXCLUDE_FROM_ALL) 
    add_subdirectory(benchmark)
endif()