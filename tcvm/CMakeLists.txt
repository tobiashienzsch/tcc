project(tcvm)

# Library
set(tcvm_lib_source
    src/tcvm/examples.hpp
    src/tcvm/examples.cpp
    src/tcvm/program_options.hpp
    
    src/tcvm/vm/vm.hpp
    src/tcvm/vm/vm.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcvm_lib_source})

add_library(tcvm_lib ${tcvm_lib_source})
add_library(tcc::tcvm ALIAS tcvm_lib)
target_include_directories(tcvm_lib PUBLIC src)
target_link_libraries(tcvm_lib  
    PUBLIC
        tcc::Core 
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
        fmt
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
        tcc::CompilerOptions
        tcc::CompilerWarnings   
)

# Test
if(TCC_BUILD_TESTS)
    add_subdirectory(test)
endif()