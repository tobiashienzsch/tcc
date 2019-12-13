project(tcc-tests)

set (TEST_SRC
    test_main.cpp
    
    test_warnings.cpp

    test_ast.cpp
    test_lexer.cpp
    test_vm.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SRC})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}
    tcc::Library
    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})