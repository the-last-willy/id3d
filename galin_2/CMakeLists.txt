add_executable(galin_2_mesh src/application/mesh/main.cpp)
target_include_directories(galin_2_mesh PRIVATE src)
target_link_libraries(galin_2_mesh PRIVATE common)
