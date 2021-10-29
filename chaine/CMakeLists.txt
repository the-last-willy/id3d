add_library(chaine_common INTERFACE)
target_include_directories(chaine_common
INTERFACE
    src
)
target_link_libraries(chaine_common
INTERFACE
    common
)

add_executable(chaine_2d src/application/2d/main.cpp)
target_link_libraries(chaine_2d
PRIVATE
    chaine_common
)

add_executable(chaine_3d src/application/3d/main.cpp)
target_link_libraries(chaine_3d
PRIVATE
    chaine_common
)
