add_executable(iehl src/main.cpp)
target_include_directories(iehl PRIVATE src)
target_link_libraries(iehl PRIVATE common)

# add_executable(iehl1_debug src/debug.cpp)
# target_include_directories(iehl1_debug PRIVATE src)
# target_link_libraries(iehl1_debug PRIVATE common)