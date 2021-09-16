add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glad")

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/stb_image"
    )
