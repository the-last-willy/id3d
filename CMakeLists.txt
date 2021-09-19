cmake_minimum_required(VERSION 3.15.0)
project(sandbox VERSION 0.1.0)

add_subdirectory(lib/abstractgl) # MIT
add_subdirectory(lib/dear_imgui) # MIT
add_subdirectory(lib/glad) # MIT
add_subdirectory(lib/glfw3) # ZLIB
add_subdirectory(lib/range_v3) # MIT
add_subdirectory(lib/tinygltf) # MIT
add_subdirectory(lib/tinyobjloader) # MIT

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
    tinygltf
    tinyobjloader
)

# Executables.

add_subdirectory(chaine)
add_subdirectory(iehl)

add_executable(galin src/galin/main.cpp)
target_link_libraries(galin PRIVATE common)

add_executable(dear_imgui_demo src/dear_imgui_demo.cpp)
target_link_libraries(dear_imgui_demo PRIVATE common)
