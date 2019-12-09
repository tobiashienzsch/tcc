project(tcc-lib)

# Add library
add_library(${PROJECT_NAME} INTERFACE)
add_library(tcc::Library ALIAS ${PROJECT_NAME})

# Add include directories
target_include_directories( ${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})

# Link
target_link_libraries(${PROJECT_NAME} INTERFACE
    # tcc::CodeCoverage
    # tcc::CompilerOptions          # Language standard, etc...
)