project(tcc)

# library
set(tcc_lib_source
    # src/asm/generator.cpp
    src/optimizer/optimizer.hpp

    src/ir/intermediate_representation.cpp
    src/ir/intermediate_representation.hpp
    src/ir/three_address_code.cpp
    src/ir/three_address_code.hpp
)

add_library(tcc_lib STATIC ${tcc_lib_source})
add_library(tcc::tcc ALIAS tcc_lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})
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
    
    src/ast.hpp
    src/ast_adapted.hpp
    src/common.hpp
    src/compiler.cpp
    src/compiler.hpp
    src/config.hpp
    src/error_handler.hpp
    src/expression_def.hpp
    src/expression.cpp
    src/expression.hpp
    src/statement_def.hpp
    src/statement.cpp
    src/statement.hpp
    src/vm.cpp
    src/vm.hpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_source})
add_executable(${PROJECT_NAME} ${tcc_source})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        tcc::tcc
        tcc::CompilerOptions
        tcc::CompilerWarnings

)

# test
if(TCC_BUILD_TESTS)
    add_subdirectory(test)
endif()