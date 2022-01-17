option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glad")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gflags)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/stb_image"
        "${CMAKE_CURRENT_SOURCE_DIR}/glm"
    PRIVATE
    )
