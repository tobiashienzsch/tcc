project(playground-minic)

set(minic_sources
    src/annotation.hpp
    src/ast.hpp
    src/compiler.cpp
    src/compiler.hpp
    src/error_handler.hpp
    src/expression.cpp
    src/expression.hpp
    src/expression_def.hpp
    src/function.cpp
    src/function.hpp
    src/function_def.hpp
    src/skipper.hpp
    src/statement.cpp
    src/statement.hpp
    src/statement_def.hpp
    src/vm.cpp
    src/vm.hpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})

add_library(spirit_qi_minic ${minic_sources})
target_include_directories(spirit_qi_minic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(spirit_qi_minic 
    PUBLIC
        Boost::headers
    PRIVATE 
        tcc::CompilerOptions
        tcc::CompilerWarnings
)

set_target_properties(spirit_qi_minic PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        spirit_qi_minic
        tcc::CompilerOptions
        tcc::CompilerWarnings
)