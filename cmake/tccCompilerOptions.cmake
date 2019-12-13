add_library(compiler_options INTERFACE)
add_library(tcc::CompilerOptions ALIAS compiler_options)

# Language standard
target_compile_features(compiler_options INTERFACE cxx_std_17)
