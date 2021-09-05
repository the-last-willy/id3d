cmake_minimum_required(VERSION 3.15.0)
project(sandbox VERSION 0.1.0)

add_subdirectory(lib/abstractgl)
add_subdirectory(lib/dear_imgui)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw3)
add_subdirectory(lib/range_v3)
add_subdirectory(lib/tinygltf)

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
)

# Executables.

add_executable(chaine src/chaine/main.cpp)
target_link_libraries(chaine PRIVATE common)

add_executable(galin src/galin/main.cpp)
target_link_libraries(galin PRIVATE common)
