project(tcc)

# library
set(tcc_lib_source
    tcc/asm/asm_generator.cpp
    tcc/asm/asm_generator.hpp
    tcc/asm/asm_utils.cpp
    tcc/asm/asm_utils.hpp
    tcc/asm/asm.hpp

    
    tcc/compiler/options.hpp
    tcc/compiler/compiler.hpp
    tcc/compiler/program_options.hpp

    tcc/optimizer/optimizer.cpp
    tcc/optimizer/optimizer.hpp

    tcc/ir/generator.cpp
    tcc/ir/generator.hpp
    tcc/ir/instruction_set.hpp    
    tcc/ir/instruction_set.cpp    
    tcc/ir/statement.cpp
    tcc/ir/statement.hpp

    tcc/parser/annotation.hpp
    tcc/parser/ast.hpp
    tcc/parser/error_handler.hpp
    tcc/parser/expression.cpp
    tcc/parser/expression_def.hpp
    tcc/parser/expression.hpp
    tcc/parser/function.cpp
    tcc/parser/function_def.hpp
    tcc/parser/function.hpp
    tcc/parser/parser.hpp
    tcc/parser/skipper.hpp
    tcc/parser/statement.cpp
    tcc/parser/statement_def.hpp
    tcc/parser/statement.hpp
    tcc/parser/testing.hpp
)

add_library(tcc_lib ${tcc_lib_source})
add_library(tcc::tcc ALIAS tcc_lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_source})
set_target_properties(tcc_lib PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_include_directories(tcc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tcc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tcc)
target_link_libraries(tcc_lib 
    PUBLIC 
        tcc::tcsl
        
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)

# exe
set(tcc_source 
    main.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_source})
add_executable(${PROJECT_NAME} ${tcc_source})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

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

        tcc/asm/asm_utils_test.cpp

        tcc/compiler/program_options_test.cpp

        tcc/optimizer/optimizer_test.cpp

        tcc/parser/expression_test.cpp
        tcc/parser/function_test.cpp
        tcc/parser/parser_test.cpp
        tcc/parser/statement_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_test_source})

    add_executable(tests-tcc ${tcc_test_source})

    target_link_libraries(tests-tcc
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
    catch_discover_tests(tests-tcc)
endif()