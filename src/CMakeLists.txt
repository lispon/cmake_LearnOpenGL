file(GLOB HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/file/*.glsl)

message("GLSL: ${GLSL}")
add_executable(${PROJECT_NAME}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${HDR_FILES}
        ${SRC_FILES}
        ${GLSL_FILES}
)

source_group(GLSL
    FILES
        ${GLSL_FILES}
)


# 复制着色器文件到可执行目录的./shader文件夹内.
# 注意, 此时的 ${CMAKE_CURRENT_BINARY_DIR} 和 ${CMAKE_BINARY_DIR} 输出的值不同.
# 准确定位输出的可执行文件路径,
# 需要在调用 add_executable 的cmakelists.txt 中使用 ${CMAKE_CURRENT_BINARY_DIR} 获取.

# 着色器文件.
file(COPY
    ${PROJECT_SOURCE_DIR}/src/file/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader
    FILES_MATCHING PATTERN "*.glsl"
    )

# 纹理图片.
file(COPY ${PROJECT_SOURCE_DIR}/src/img
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    FILES_MATCHING PATTERN "*.jpg" PATTERN "*.png"
    )
