cmake_minimum_required(VERSION 3.12...3.16)
project(TobanteCrappyCompilerCollection
    VERSION 0.1
    DESCRIPTION "tobante's (crappy) compiler collection"
    LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
    set(CMAKE_CXX_EXTENSIONS NO)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_SHARED_LIBS OFF)
    
    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()

    include(CheckIPOSupported)
    check_ipo_supported(RESULT TCC_BUILD_LTO)
    if(TCC_BUILD_LTO)
        message(STATUS "tcc: LTO is enabled.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
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
option(TCC_BUILD_PLAYGROUND "Build playground project"          ON)
option(TCC_BUILD_BENCHMARK  "Build benchmark tests"             OFF)
option(TCC_BUILD_COVERAGE   "Build with code coverage"          OFF)
option(TCC_BUILD_CLANG_TIDY "Run clang-tidy with the compiler"  OFF)

# cmake modules
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(tccCodeCoverage)
include(tccCompilerOptions)
include(tccCompilerWarnings)
include(tccThreads)

# tidy, has to be enabled for each target.
if(TCC_BUILD_CLANG_TIDY)
  if(CMake_SOURCE_DIR STREQUAL CMake_BINARY_DIR)
    message(FATAL_ERROR "TCC_BUILD_CLANG_TIDY requires an out-of-source build!")
  endif()
  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(NOT CLANG_TIDY_EXE)
    message(WARNING "TCC_BUILD_CLANG_TIDY is ON but clang-tidy is not found!")
  endif()

  # Create a preprocessor definition that depends on .clang-tidy content so
  # the compile command will change when .clang-tidy changes.  This ensures
  # that a subsequent build re-runs clang-tidy on all sources even if they
  # do not otherwise need to be recompiled.  Nothing actually uses this
  # definition.  We add it to targets on which we run clang-tidy just to
  # get the build dependency on the .clang-tidy file.
  file(SHA1 ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy clang_tidy_sha1)
  set(CLANG_TIDY_DEFINITIONS "CLANG_TIDY_SHA1=${clang_tidy_sha1}")
  unset(clang_tidy_sha1)
endif(TCC_BUILD_CLANG_TIDY)
configure_file(.clang-tidy .clang-tidy COPYONLY)

# boost
# set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.70 REQUIRED COMPONENTS date_time filesystem program_options regex serialization system)

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
add_subdirectory(tcc)
add_subdirectory(tcvm)


# benchmarks
if(TCC_BUILD_BENCHMARK)
    set(ALLOW_DOWNLOADING_GOOGLETEST FALSE)
    add_subdirectory(3rd_party/google-benchmark EXCLUDE_FROM_ALL) 
    set_target_properties(benchmark PROPERTIES FOLDER 3rd_party)
    add_subdirectory(benchmark)
endif()

# playground
if(TCC_BUILD_PLAYGROUND)
    add_subdirectory(playground)
endif()
