add_library(id3d_iehl INTERFACE)
target_include_directories(id3d_iehl INTERFACE src)
target_link_libraries(id3d_iehl INTERFACE id3d_common)

add_executable(id3d_iehl_compute src/application/compute/main.cpp)
target_include_directories(id3d_iehl_compute PRIVATE src)
target_link_libraries(id3d_iehl_compute PRIVATE id3d_iehl)

add_executable(id3d_iehl_renderer src/application/renderer/main.cpp)
target_include_directories(id3d_iehl_renderer PRIVATE src)
target_link_libraries(id3d_iehl_renderer PRIVATE id3d_iehl)
