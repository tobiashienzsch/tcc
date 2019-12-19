project(tcc-playground)

# BOOST FUSION
add_executable(${PROJECT_NAME}_boost_fusion  main_boost_fusion.cpp)
target_compile_features(${PROJECT_NAME}_boost_fusion PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}_boost_fusion PRIVATE Boost::boost)

add_subdirectory(spirit_parser)