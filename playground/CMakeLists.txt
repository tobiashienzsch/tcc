project(playground)

add_library(playground_lib)
target_sources(playground_lib PUBLIC
        ast.hpp
        ast.cpp
        lexer.hpp
        parse_context.hpp
        parser.hpp
        token.hpp
        token.cpp
        )

target_link_libraries(playground_lib
        PUBLIC
        fmt
        Boost::headers
        tcc::CompilerOptions
        tcc::CompilerWarnings
        PRIVATE
        tcc::CodeCoverage
        )

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC playground_lib)

if (TCC_BUILD_TESTS)
    add_executable(playground_test main_test.cpp lexer_test.cpp)
    target_link_libraries(playground_test
            PRIVATE
            playground_lib
            tcc::CodeCoverage
            Catch2::Catch2
            )


    include(CTest)
    include(Catch)
    catch_discover_tests(playground_test)
endif ()