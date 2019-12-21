project(tcc-core)

set(tcc_core_source
    tcc/core/core.hpp
    tcc/core/testing.hpp
    tcc/core/binary_format.hpp
    tcc/core/binary_format.cpp
)    
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcc_core_source})

add_library(${PROJECT_NAME} STATIC ${tcc_core_source})
add_library(tcc::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        fmt
    PRIVATE
        tcc::CompilerOptions
        tcc::CompilerWarnings
        tcc::CodeCoverage

)