add_library(id3d_chaine_common INTERFACE)
target_include_directories(id3d_chaine_common INTERFACE src)
target_link_libraries(id3d_chaine_common INTERFACE id3d_common)

add_executable(id3d_chaine_2d src/application/2d/main.cpp)
target_link_libraries(id3d_chaine_2d PRIVATE id3d_chaine_common)

add_executable(id3d_chaine_3d src/application/3d/main.cpp)
target_link_libraries(id3d_chaine_3d PRIVATE id3d_chaine_common)

add_custom_target(id3d_chaine_all)
add_dependencies(id3d_chaine_all id3d_chaine_2d id3d_chaine_3d)
