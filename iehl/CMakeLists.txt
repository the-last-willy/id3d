add_executable(iehl src/main.cpp)
target_include_directories(iehl PRIVATE src)
target_link_libraries(iehl PRIVATE common)
