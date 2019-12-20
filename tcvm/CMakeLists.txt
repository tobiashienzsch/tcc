project(tcvm)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        tcc::Library
        tcc::CompilerOptions
        tcc::CompilerWarnings    
        Boost::boost 
        Boost::program_options 
        Boost::system
        Boost::filesystem
        fmt
)

# LTO
if(TCC_BUILD_LTO)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()