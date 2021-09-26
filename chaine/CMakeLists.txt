add_executable(chaine2d src/main2d.cpp)

target_include_directories(chaine2d
PRIVATE
    src
)

target_link_libraries(chaine2d
PRIVATE
    common
)

add_executable(chaine3d src/main3d.cpp)

target_include_directories(chaine3d
PRIVATE
    src
)

target_link_libraries(chaine3d
PRIVATE
    common
)

