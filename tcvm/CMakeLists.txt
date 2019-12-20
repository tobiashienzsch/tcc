project(tcvm)

set(tcvm_source
    main.cpp

    src/tcvm/examples.hpp
    src/tcvm/program_options.hpp
)

add_executable(${PROJECT_NAME} ${tcvm_source})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tcvm_source})
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
