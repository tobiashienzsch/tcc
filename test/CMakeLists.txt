project(tcc-tests)

set (TEST_SRC
    test_main.cpp
    
    test_warnings.cpp

    test_ast.cpp
    test_byte_code.cpp
    test_lexer.cpp
    test_parser.cpp
    test_parser_x3.cpp
    test_vm.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME}
PRIVATE
    tcc::CompilerOptions
    tcc::CompilerWarnings
    tcc::CodeCoverage
    tcc::Library
    Catch2::Catch2
)

# LTO
if(TCC_BUILD_LTO)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})