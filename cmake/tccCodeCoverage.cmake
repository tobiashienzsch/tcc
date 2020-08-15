add_library(tcc_coverage INTERFACE)
add_library(tcc::CodeCoverage ALIAS tcc_coverage)

if(TCC_BUILD_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(tcc_coverage INTERFACE -O0 -g --coverage)
  target_link_libraries(tcc_coverage INTERFACE --coverage)
endif()

if(TCC_BUILD_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(tcc_coverage INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_libraries(tcc_coverage INTERFACE --coverage)
endif()