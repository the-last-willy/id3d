cmake_minimum_required(VERSION 3.15.0)
project(sandbox VERSION 0.1.0)

add_subdirectory(lib/abstractgl)
add_subdirectory(lib/dear_imgui)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw3)
add_subdirectory(lib/range_v3)
add_subdirectory(lib/stb)
add_subdirectory(lib/tinygltf)
add_subdirectory(lib/tinyobjloader)

if(MSVC)
    add_compile_options(/std:c++latest /W4)
elseif()
    add_compile_options(/std=c++20 /Wall)
endif()

# Library 'common'.

add_library(common INTERFACE)

target_include_directories(common
INTERFACE
    src
)

target_link_libraries(common
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

# Executables.

add_subdirectory(chaine)
add_subdirectory(galin)
add_subdirectory(galin_2)
add_subdirectory(iehl)

add_executable(dear_imgui_demo src/dear_imgui_demo.cpp)
target_link_libraries(dear_imgui_demo PRIVATE common)
