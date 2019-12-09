cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(TOBANTE_CRAPPY_COMPILER_BUILD_TESTS "Build the unit tests with Catch2" ON)
project(TobanteCrappyCompiler)

add_subdirectory(lib)
add_subdirectory(src)

# Testing
if(TOBANTE_CRAPPY_COMPILER_BUILD_TESTS)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
    enable_testing()
    add_subdirectory(3rd_party/Catch2 EXCLUDE_FROM_ALL)
    include(CTest)
    include(Catch)
    add_subdirectory(test)
endif()