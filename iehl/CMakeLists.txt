add_library(iehl_common INTERFACE)
target_include_directories(iehl_common INTERFACE src)
target_link_libraries(iehl_common INTERFACE common)

add_executable(iehl_renderer src/application/renderer/main.cpp)
target_include_directories(iehl_renderer PRIVATE src)
target_link_libraries(iehl_renderer PRIVATE iehl_common)
