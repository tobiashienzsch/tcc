project(tcc2)

# library
set(tcc_lib_2_source
    src/expression.cpp
    src/interpreter.cpp
    src/printer.cpp
)

add_library(tcc_lib_2 STATIC ${tcc_lib_2_source})
add_library(tcc::tcc2 ALIAS tcc_lib_2)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_lib_2_source})
# set_target_properties(tcc_lib_2 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(tcc_lib_2 PUBLIC src)
target_link_libraries(tcc_lib_2 
    PUBLIC 
        tcc::Core
        
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
        tcc::tcc2
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage
)

# test
if(TCC_BUILD_TESTS)
    add_subdirectory(test)
endif()