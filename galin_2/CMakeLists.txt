add_library(galin_2_common INTERFACE)
target_include_directories(galin_2_common INTERFACE src)
target_link_libraries(galin_2_common INTERFACE common)

add_executable(galin_2_revolution src/application/revolution/main.cpp)
target_link_libraries(galin_2_revolution PRIVATE galin_2_common)

add_executable(galin_2_teaset src/application/teaset/main.cpp)
target_link_libraries(galin_2_teaset PRIVATE galin_2_common)
