add_executable(chaine src/main.cpp)

target_include_directories(chaine
PRIVATE
    src
)

target_link_libraries(chaine
PRIVATE
    common
)
