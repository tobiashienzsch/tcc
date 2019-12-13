# Code Coverage Configuration
add_library(tcc_coverage INTERFACE)
add_library(tcc::CodeCoverage ALIAS tcc_coverage)

if(TCC_BUILD_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(tcc_coverage INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(tcc_coverage INTERFACE --coverage)
  else()
    target_link_libraries(tcc_coverage INTERFACE --coverage)
  endif()
endif()