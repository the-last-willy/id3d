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

add_subdirectory(common)

add_subdirectory(chaine)
add_subdirectory(galin)
add_subdirectory(galin_2)
add_subdirectory(iehl)
