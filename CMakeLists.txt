cmake_minimum_required(VERSION 3.12...3.16)
project(TobanteCrappyCompilerCollection
    VERSION 0.1
    DESCRIPTION "tobante's (crappy) compiler collection"
    LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_SHARED_LIBS OFF)
    
    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT TCC_BUILD_LTO)
    if(TCC_BUILD_LTO)
        message(STATUS "TobanteCrappyCompilerCollection: lto is supported, but not used.")
        # set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    # Docs only available if this is the main app
    find_package(Doxygen)
        if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
	    message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

# project options
option(TCC_BUILD_TESTS      "Build the unit tests with Catch2"  ON)
option(TCC_BUILD_BENCHMARK  "Build benchmark tests"             ON)
option(TCC_BUILD_COVERAGE   "Build with code coverage"          OFF)

# cmake modules
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(tccCodeCoverage)
include(tccCompilerOptions)
include(tccCompilerWarnings)

# boost
find_package(Boost 1.65 REQUIRED COMPONENTS system program_options filesystem)

# fmt
add_subdirectory(3rd_party/fmt EXCLUDE_FROM_ALL)
set_target_properties(fmt PROPERTIES FOLDER 3rd_party)

# catch2
if(TCC_BUILD_TESTS)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
    enable_testing()
    add_subdirectory(3rd_party/Catch2 ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/Catch2 EXCLUDE_FROM_ALL)
    include(CTest)
    include(Catch)
endif()

# projects
add_subdirectory(lib/core)
# add_subdirectory(lib)
add_subdirectory(tcc)
add_subdirectory(tcvm)


# benchmarks
if(TCC_BUILD_BENCHMARK)
    set(ALLOW_DOWNLOADING_GOOGLETEST FALSE)
    add_subdirectory(3rd_party/google-benchmark EXCLUDE_FROM_ALL) 
    set_target_properties(benchmark PROPERTIES FOLDER 3rd_party)
    add_subdirectory(benchmark)
endif()

