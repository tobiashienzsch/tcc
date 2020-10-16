cmake_minimum_required(VERSION 3.12...3.16)
project(TobanteCrappyCompilerCollection
    VERSION 0.1.0
    DESCRIPTION "tobante's (crappy) compiler collection"
    LANGUAGES CXX
)

# LTO
cmake_policy(SET CMP0069 NEW) 
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# project options
option(TCC_BUILD_TESTS      "Build the unit tests with Catch2"  ON)
option(TCC_BUILD_WERROR     "Build with warnings as errors"     OFF)
option(TCC_BUILD_COVERAGE   "Build with code coverage"          OFF)
option(TCC_BUILD_LTO        "Build with lto enabled"            OFF)
option(TCC_BUILD_PLAYGROUND "Build playground project"          ON)
option(TCC_BUILD_BENCHMARK  "Build benchmark tests"             OFF)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to conform to")
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
    set(CMAKE_CXX_EXTENSIONS NO)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_SHARED_LIBS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-mavx" COMPILER_SUPPORTS_MAVX)
    if(COMPILER_SUPPORTS_MAVX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    endif()

    if(TCC_BUILD_LTO)
      include(CheckIPOSupported)
      check_ipo_supported(RESULT COMPILER_SUPPORTS_LTO)
      if(COMPILER_SUPPORTS_LTO)
          message(STATUS "tcc: LTO is enabled.")
          set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
      endif()
    endif(TCC_BUILD_LTO)

    # Docs only available if this is the main app
    find_package(Doxygen)
        if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
	    message(STATUS "Doxygen not found, not building docs")
    endif()

    # Install directory
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

endif()


# cmake modules
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(tccCodeCoverage)
include(tccCompilerOptions)
include(tccCompilerWarnings)
include(tccThreads)

# tidy, has to be enabled for each target.
if(TCC_CLANG_TIDY)
  if(CMake_SOURCE_DIR STREQUAL CMake_BINARY_DIR)
    message(FATAL_ERROR "TCC_CLANG_TIDY requires an out-of-source build!")
  endif()

  find_program(CLANG_TIDY_EXE NAMES clang-tidy)
  if(NOT CLANG_TIDY_EXE)
    message(WARNING "TCC_CLANG_TIDY is ON but clang-tidy is not found!")
    set(DO_CLANG_TIDY "")
  else()
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
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
endif(TCC_CLANG_TIDY)
configure_file(.clang-tidy .clang-tidy COPYONLY)

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.71 REQUIRED COMPONENTS date_time filesystem program_options regex serialization system)

add_subdirectory(3rd_party/fmt EXCLUDE_FROM_ALL)

# catch2
if(TCC_BUILD_TESTS)
    list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/Catch2/contrib")
    enable_testing()
    add_subdirectory(3rd_party/Catch2 ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/Catch2 EXCLUDE_FROM_ALL)
    include(CTest)
    include(Catch)
endif()

# sub projects
add_subdirectory(tcsl)
add_subdirectory(tcc)
add_subdirectory(tcvm)
add_subdirectory(tests)


# benchmarks
if(TCC_BUILD_BENCHMARK)
    set(ALLOW_DOWNLOADING_GOOGLETEST FALSE CACHE BOOL "")
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "")
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
    add_subdirectory(3rd_party/google-benchmark EXCLUDE_FROM_ALL) 
    set_target_properties(benchmark PROPERTIES FOLDER 3rd_party)
    add_subdirectory(benchmark)
endif()

# playground
if(TCC_BUILD_PLAYGROUND)
    add_subdirectory(playground)
endif()
