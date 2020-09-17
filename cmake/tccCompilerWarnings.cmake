add_library(compiler_warnings INTERFACE)
add_library(tcc::CompilerWarnings ALIAS compiler_warnings)

if(MSVC)
  target_compile_options(compiler_warnings INTERFACE /W3)
  if(TCC_BUILD_WERROR)
    target_compile_options(compiler_warnings INTERFACE /WX)
  endif(TCC_BUILD_WERROR)
else()
  if(TCC_BUILD_WERROR)
    target_compile_options(compiler_warnings INTERFACE -Werror)
  endif(TCC_BUILD_WERROR)

  # GCC & CLANG
  target_compile_options(
    compiler_warnings
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wdouble-promotion
        -Wno-sign-conversion
  )
  # GCC 
  target_compile_options(
    compiler_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:
        -Wduplicated-branches
        -Wduplicated-cond
        -Wlogical-op
        -Wnull-dereference
        -Wno-sequence-point
        -Wno-parentheses
    >
  )

  # CLANG 
  target_compile_options(
    compiler_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:Clang>:
        -Wshadow-all
        -Wlogical-op-parentheses
        -Wmisleading-indentation
        -Wloop-analysis
    >
  )
endif()
