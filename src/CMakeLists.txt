set(HDR_FILES
    openglwindowbase.h
    shader.h
    startshader.h
    )

set(SRC_FILES
    openglwindowbase.cpp
    main.cpp
    shader.cpp
    startshader.cpp
    )

add_executable(${PROJECT_NAME}
    ${HDR_FILES}
    ${SRC_FILES}
    )


# 复制着色器文件到可执行目录的./shader文件夹内.
# 注意, 此时的 ${CMAKE_CURRENT_BINARY_DIR} 和 ${CMAKE_BINARY_DIR} 输出的值不同.
# 准确定位输出的可执行文件路径,
# 需要在调用 add_executable 的cmakelists.txt 中使用 ${CMAKE_CURRENT_BINARY_DIR} 获取.
file(COPY
    ${PROJECT_SOURCE_DIR}/src/file/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader
    FILES_MATCHING PATTERN "*.fs" PATTERN "*.vs"
    )
