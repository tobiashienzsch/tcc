project(tcc-core)

set(tcc_core_source
    tcc/core.hpp
    tcc/core/binary_format.hpp
    tcc/core/binary_format.cpp
    tcc/core/byte_code.hpp
    tcc/core/byte_code.cpp
    tcc/core/testing.hpp
    tcc/core/variant.hpp
    tcc/core/warning.hpp
)    
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_core_source})

add_library(${PROJECT_NAME} STATIC ${tcc_core_source})
add_library(tcc::Core ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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
    add_subdirectory(test)
endif()