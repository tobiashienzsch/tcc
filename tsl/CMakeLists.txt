project(tsl)

set(tsl_source
    tsl/tsl.hpp
    tsl/binary_format.hpp
    tsl/binary_format.cpp
    tsl/byte_code.hpp
    tsl/byte_code.cpp
    tsl/testing.hpp
    tsl/variant.hpp
    tsl/warning.hpp
)    
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tsl_source})

add_library(${PROJECT_NAME} STATIC ${tsl_source})
add_library(tcc::tsl ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rd_party/gsl/include
)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Boost::serialization
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage

)

# Test
if(TCC_BUILD_TESTS)
    set (tsl_test_source
        main_test.cpp
        tsl/binary_format_test.cpp
        tsl/byte_code_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tsl_test_source})

    add_executable(tsl-tests ${tsl_test_source})
    target_link_libraries(tsl-tests
    PRIVATE
        tcc::tsl
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
        Boost::filesystem
        Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(tsl-tests)
endif()