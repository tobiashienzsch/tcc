project(tcc)

# library
set(tcc_lib_source
    tcc/asm/assembly_generator.cpp
    tcc/asm/assembly_generator.hpp

    tcc/optimizer/optimizer.cpp
    tcc/optimizer/optimizer.hpp

    tcc/ir/generator.cpp
    tcc/ir/generator.hpp
    tcc/ir/intermediate_representation.cpp
    tcc/ir/intermediate_representation.hpp
    tcc/ir/three_address_code.cpp
    tcc/ir/three_address_code.hpp

    tcc/parser/ast.hpp
    tcc/parser/ast_adapted.hpp
    tcc/parser/ast_printer.cpp
    tcc/parser/ast_printer.hpp
    tcc/parser/common.hpp
    tcc/parser/compiler.cpp
    tcc/parser/compiler.hpp
    tcc/parser/config.hpp
    tcc/parser/error_handler.hpp
    tcc/parser/expression_def.hpp
    tcc/parser/expression.hpp
    tcc/parser/statement_def.hpp
    tcc/parser/statement.cpp
    tcc/parser/statement.hpp

    tcc/parser-qi/annotation.hpp
    tcc/parser-qi/ast.hpp
    tcc/parser-qi/compiler.cpp
    tcc/parser-qi/compiler.hpp
    tcc/parser-qi/error_handler.hpp
    tcc/parser-qi/expression.cpp
    tcc/parser-qi/expression_def.hpp
    tcc/parser-qi/expression.hpp
    tcc/parser-qi/function.cpp
    tcc/parser-qi/function_def.hpp
    tcc/parser-qi/function.hpp
    tcc/parser-qi/skipper.hpp
    tcc/parser-qi/statement.cpp
    tcc/parser-qi/statement_def.hpp
    tcc/parser-qi/statement.hpp
    tcc/parser-qi/vm.cpp
    tcc/parser-qi/vm.hpp

    tcc/vm.cpp
    tcc/vm.hpp    
)

add_library(tcc_lib ${tcc_lib_source})
add_library(tcc::tcc ALIAS tcc_lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})
set_target_properties(tcc_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(tcc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tcc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tcc)
target_link_libraries(tcc_lib 
    PUBLIC 
        tcc::tsl
        
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
    main_neo.cpp
    tcc/parser/expression.cpp

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
    set (tcc_test_source
        main_test.cpp
        tcc/parser/expression.cpp

        tcc/optimizer/optimizer_test.cpp
        tcc/parser/common_test.cpp
        tcc/parser/statement_test.cpp
        tcc/parser-qi/expression_test.cpp
        tcc/parser-qi/statement_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_test_source})

    add_executable(tcc_tests ${tcc_test_source})
    target_link_libraries(tcc_tests
        PRIVATE
            tcc::tcc
            tcc::CompilerOptions
            tcc::CompilerWarnings
            tcc::CodeCoverage
            Boost::boost 
            Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(tcc_tests)
endif()