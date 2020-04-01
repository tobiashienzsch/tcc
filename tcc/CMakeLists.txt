project(tcc)

# library
set(tcc_lib_source
    src/asm/assembly_generator.cpp
    src/asm/assembly_generator.hpp

    src/optimizer/optimizer.cpp
    src/optimizer/optimizer.hpp

    src/ir/intermediate_representation.cpp
    src/ir/intermediate_representation.hpp
    src/ir/three_address_code.cpp
    src/ir/three_address_code.hpp
)

add_library(tcc_lib STATIC ${tcc_lib_source})
add_library(tcc::tcc ALIAS tcc_lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})
set_target_properties(tcc_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(tcc_lib PUBLIC src)
target_link_libraries(tcc_lib 
    PUBLIC 
        tcc::Core
        
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage

)

# exe
set(tcc_source 
    main.cpp
    
    src/parser/ast.hpp
    src/parser/ast_adapted.hpp
    src/parser/common.hpp
    src/parser/compiler.cpp
    src/parser/compiler.hpp
    src/parser/config.hpp
    src/parser/error_handler.hpp
    src/parser/expression_def.hpp
    src/parser/expression.cpp
    src/parser/expression.hpp
    src/parser/statement_def.hpp
    src/parser/statement.cpp
    src/parser/statement.hpp
    src/vm.cpp
    src/vm.hpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_source})
add_executable(${PROJECT_NAME} ${tcc_source})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        tcc::tcc
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)

# test
if(TCC_BUILD_TESTS)
    add_subdirectory(test)
endif()