project(tcsl)

set(tcsl_source
    tcsl/tcsl.hpp
    tcsl/binary_format.hpp
    tcsl/binary_format.cpp
    tcsl/file.hpp
    tcsl/byte_code.hpp
    tcsl/byte_code.cpp
    tcsl/testing.hpp
    tcsl/variant.hpp
    tcsl/warning.hpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcsl_source})

add_library(${PROJECT_NAME} ${tcsl_source})
add_library(tcc::tcsl ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rd_party/gsl/include
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Boost::serialization
        Boost::filesystem
        fmt::fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)

# Test
if(TCC_BUILD_TESTS)
    set (tcsl_test_source
        main_test.cpp
        tcsl/binary_format_test.cpp
        tcsl/byte_code_test.cpp
        tcsl/file_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcsl_test_source})

    add_executable(tests-tcsl ${tcsl_test_source})

    target_link_libraries(tests-tcsl
    PRIVATE
        tcc::tcsl
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
        Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(tests-tcsl)
endif()