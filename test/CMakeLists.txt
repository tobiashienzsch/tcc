project(tcc-tests)

set (TEST_SRC
    test_main.cpp
    test_warnings.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME}
    # tcc::CompilerOptions
    # tcc::CompilerWarnings
    tcc::Library

    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})