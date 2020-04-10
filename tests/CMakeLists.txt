project(tests-integration)
# test
if(TCC_BUILD_TESTS)
    set (tcc_test_source
        main_test.cpp
        integration_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_test_source})
    add_executable(${PROJECT_NAME} ${tcc_test_source})
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            tcc::tcc
            tcc::tcvm
            tcc::CompilerOptions
            tcc::CompilerWarnings
            tcc::CodeCoverage
            Boost::boost 
            Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(${PROJECT_NAME})
endif()