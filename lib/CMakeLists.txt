project(tcc-lib)

# Add library
set(TCC_LIB_SOURCES
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/ast.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/ast.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/lexer.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/lexer.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/parser.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/compiler_old/parser_x3.hpp"

    "${PROJECT_SOURCE_DIR}/tcc/vm/byte_code.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/byte_code.cpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/vm.hpp"
    "${PROJECT_SOURCE_DIR}/tcc/vm/vm.cpp"

)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TCC_LIB_SOURCES})

add_library(${PROJECT_NAME} STATIC ${TCC_LIB_SOURCES})

add_library(tcc::Library ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} 
PUBLIC
    tcc::Core

    Boost::boost
    Boost::system
PRIVATE
    tcc::CompilerOptions
    tcc::CompilerWarnings
    tcc::CodeCoverage
)