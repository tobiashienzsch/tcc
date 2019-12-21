project(tcc-playground)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::serialization tcc::Core tcc::CompilerOptions tcc::CompilerWarnings)