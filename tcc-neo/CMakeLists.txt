project(tcc-neo)

# library
set(tcc_neo_lib_source
    tcc-neo/parser/package.hpp
    tcc-neo/parser/package.cpp
    tcc-neo/parser/identifier.hpp
    tcc-neo/parser/identifier.cpp
)

add_library(tcc_neo_lib STATIC ${tcc_neo_lib_source})
add_library(tcc::tccNeo ALIAS tcc_neo_lib)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_neo_lib_source})
# set_target_properties(tcc_neo_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(tcc_neo_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tcc_neo_lib 
    PUBLIC 
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::regex
        Boost::filesystem
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage

)

# exe
set(tcc_source main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_source})
add_executable(${PROJECT_NAME} ${tcc_source})
# set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        tcc::tccNeo
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)

# test
if(TCC_BUILD_TESTS)
    set (TEST_SRC
        main_test.cpp
        tcc-neo/parser/package_test.cpp
        tcc-neo/parser/identifier_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_SRC})

    add_executable(tcc-neo-tests ${TEST_SRC})
    target_link_libraries(tcc-neo-tests
        PRIVATE
            tcc::tccNeo
            tcc::CompilerOptions
            tcc::CompilerWarnings
            tcc::CodeCoverage
            Boost::boost 
            Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(tcc-neo-tests)
endif()