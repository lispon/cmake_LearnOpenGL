cmake_minimum_required(VERSION 3.10)

project(cmake_LearnOpenGL VERSION 1.0.0)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_gl_LIBRARY}
    glad glfw
    )
