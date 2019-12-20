project(tcc-test)

set (TEST_SRC
    test_main.cpp
    
    test_parser_x3.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_SRC})

add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME}
PRIVATE
    tcc::CompilerOptions
    tcc::CompilerWarnings
    tcc::CodeCoverage
    Boost::boost 
    Catch2::Catch2
)


include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})