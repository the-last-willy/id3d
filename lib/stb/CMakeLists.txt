add_library(stb_common INTERFACE)
target_include_directories(stb_common 
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

add_library(stb_image)
target_sources(stb_image
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.c
)
target_link_libraries(stb_image 
PUBLIC
    stb_common
)

add_library(stb_image_resize)
target_sources(stb_image_resize
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image_resize.c
)
target_link_libraries(stb_image_resize
PUBLIC
    stb_common
)

add_library(stb_all INTERFACE)
target_link_libraries(stb_all
INTERFACE
    stb_image
    stb_image_resize
)
