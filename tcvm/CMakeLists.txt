project(tcvm)

# Library
set(tcvm_lib_source
    tcvm/examples.hpp
    tcvm/examples.cpp
    tcvm/program_options.hpp
    
    tcvm/vm/vm.hpp
    tcvm/vm/vm.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcvm_lib_source})

add_library(tcvm_lib ${tcvm_lib_source})
add_library(tcc::tcvm ALIAS tcvm_lib)
set_target_properties(tcvm_lib PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_include_directories(tcvm_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tcvm_lib  
    PUBLIC
        tcc::tsl 
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
    PRIVATE 
        tcc::CompilerOptions
        tcc::CompilerWarnings  
        tcc::CodeCoverage 
)

# Executable
set(tcvm_source main.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcvm_source})
add_executable(${PROJECT_NAME} ${tcvm_source})

target_link_libraries(${PROJECT_NAME}  
    PUBLIC 
        tcc::tcvm
    PRIVATE 
        tcc::CodeCoverage
        tcc::CompilerOptions
        tcc::CompilerWarnings   
)

# Test
if(TCC_BUILD_TESTS)
    set (tcvm_test_source
        main_test.cpp
        tcvm/vm/vm_test.cpp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcvm_test_source})

    add_executable(tcvm-tests ${tcvm_test_source})
    target_link_libraries(tcvm-tests
        PRIVATE
            tcc::tcvm
            tcc::CompilerOptions
            tcc::CompilerWarnings
            tcc::CodeCoverage
            Catch2::Catch2
    )


    include(CTest)
    include(Catch)
    catch_discover_tests(tcvm-tests)
endif()