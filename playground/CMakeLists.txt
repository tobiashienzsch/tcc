project(tcc-playground)

set(playground_source
    main.cpp
)

add_executable(${PROJECT_NAME} ${playground_source})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${playground_source})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Boost::boost 
        Boost::system 
        Boost::filesystem
        Boost::serialization 
        tcc::Core
        tcc::Threads 
        tcc::CompilerOptions 
        tcc::CompilerWarnings
)