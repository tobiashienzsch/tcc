project(tcc-lib)

# Add library
add_library(${PROJECT_NAME} STATIC 
    ${PROJECT_SOURCE_DIR}/tcc/compiler/lexer.cpp
)

add_library(tcc::Library ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PUBLIC
    # tcc::CodeCoverage
    # tcc::CompilerOptions          # Language standard, etc...
)