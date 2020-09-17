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
        -Wshadow # fmt has a lot of shadow problems
        # -Wsign-conversion
        # -Wdouble-promotion
        # -Wconversion
  )
  # GCC 
  target_compile_options(
    compiler_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:
        -Wmisleading-indentation
        -Wlogical-op
        -Wduplicated-branches
        -Wduplicated-cond
        -Wno-parentheses
        -Wno-sequence-point
    >
  )

  # CLANG 
  target_compile_options(
    compiler_warnings
    INTERFACE
    $<$<CXX_COMPILER_ID:Clang>:
        -Wshadow-all # fmt has a lot of shadow problems
    >
  )
endif()
