add_library(id3d_common INTERFACE)
target_include_directories(id3d_common INTERFACE src)
target_link_libraries(id3d_common
INTERFACE
    abstractgl
    dear_imgui
    glad
    glfw
    range-v3
    stb_all
    tinygltf
    tinyobjloader
)

add_executable(dear_imgui_demo src/id3d/common/dear_imgui_demo.cpp)
target_link_libraries(dear_imgui_demo PRIVATE id3d_common)
