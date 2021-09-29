add_executable(galin src/main.cpp)

target_include_directories(galin
PRIVATE
    src
)

target_link_libraries(galin
PRIVATE
    common
)
