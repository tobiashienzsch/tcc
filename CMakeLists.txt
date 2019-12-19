cmake_minimum_required(VERSION 3.12)
project(TobanteCrappyCompiler
    VERSION 0.1
    DESCRIPTION "tobante's crappy compiler collection"
    LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Docs only available if this is the main app
    find_package(Doxygen)
        if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
	    message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

option(TCC_BUILD_TESTS      "Build the unit tests with Catch2"  ON)
option(TCC_BUILD_BENCHMARK  "Build benchmark tests"             ON)
option(TCC_BUILD_COVERAGE   "Build with code coverage"          OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(tccCodeCoverage)

if (WIN32)
  set(BOOST_INCLUDEDIR          "C:\\Developer\\lib\\boost"         CACHE PATH "Boost include path")
  set(BOOST_LIBRARYDIR          "C:\\Developer\\lib\\boost"         CACHE PATH "Boost lib path")
  set(BOOST_ROOT                "C:\\Developer\\lib\\boost\\boost"  CACHE PATH "Boost library path")
  set(Boost_NO_SYSTEM_PATHS     on                                  CACHE BOOL "Do not search system for Boost")
endif()

find_package(Boost 1.65 REQUIRED COMPONENTS system program_options)

add_subdirectory(3rd_party/fmt EXCLUDE_FROM_ALL)

add_subdirectory(lib)
add_subdirectory(tcc)
add_subdirectory(tcvm)

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

