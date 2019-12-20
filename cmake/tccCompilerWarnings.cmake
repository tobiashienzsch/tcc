add_library(compiler_warnings INTERFACE)
add_library(tcc::CompilerWarnings ALIAS compiler_warnings)

if(MSVC)
  target_compile_options(compiler_warnings INTERFACE /W3 "/permissive-")
else()
  # GCC & CLANG
  target_compile_options(
    compiler_warnings
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        # -Wsign-conversion
        # -Wdouble-promotion
        # -Wconversion
  )
  # GCC ONLY
  target_compile_options(
    compiler_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:
        -Wmisleading-indentation
        -Wlogical-op
        -Wduplicated-branches
        -Wduplicated-cond
    >
  )
endif()
