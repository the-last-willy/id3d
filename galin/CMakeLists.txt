# add_executable(galin src/main.cpp)
# target_include_directories(galin PRIVATE src)
# target_link_libraries(galin PRIVATE common)

# add_executable(galin_codegen src/codegen.cpp)
# target_include_directories(galin_codegen PRIVATE src)
# target_link_libraries(galin_codegen PRIVATE common)

add_library(id3d_galin_common INTERFACE)
target_include_directories(id3d_galin_common INTERFACE src)
target_link_libraries(id3d_galin_common INTERFACE id3d_common)

# add_executable(id3d_galin_2_revolution src/application/revolution/main.cpp)
# target_link_libraries(id3d_galin_2_revolution PRIVATE id3d_galin_2_common)

add_executable(id3d_galin_modelling src/application/teaset/main.cpp)
target_link_libraries(id3d_galin_modelling PRIVATE id3d_galin_common)

