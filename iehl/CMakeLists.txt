add_library(id3d_iehl INTERFACE)
target_include_directories(id3d_iehl INTERFACE src)
target_link_libraries(id3d_iehl INTERFACE id3d_common)

# add_executable(id3d_iehl_compute src/application/compute/main.cpp)
# target_include_directories(id3d_iehl_compute PRIVATE src)
# target_link_libraries(id3d_iehl_compute PRIVATE id3d_iehl)

# add_executable(id3d_iehl_compute2 src/application/compute/main2.cpp)
# target_include_directories(id3d_iehl_compute2 PRIVATE src)
# target_link_libraries(id3d_iehl_compute2 PRIVATE id3d_iehl)

add_executable(id3d_iehl_rasterization src/application/rasterization/main.cpp)
target_include_directories(id3d_iehl_rasterization PRIVATE src)
target_link_libraries(id3d_iehl_rasterization PRIVATE id3d_iehl)


# add_executable(id3d_iehl_renderer src/application/renderer/main.cpp)
# target_include_directories(id3d_iehl_renderer PRIVATE src)
# target_link_libraries(id3d_iehl_renderer PRIVATE id3d_iehl)
