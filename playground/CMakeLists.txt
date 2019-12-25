project(tcc-playground)

add_executable(${PROJECT_NAME} main.cpp grammar.cpp grammar.hpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost tcc::Core tcc::CompilerOptions tcc::CompilerWarnings)