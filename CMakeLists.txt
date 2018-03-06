# 最小版本
cmake_minimum_required(VERSION 2.8)
# 项目名称
project(cg_opengl)
# 查找依赖包
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
# 查找失败
if(NOT ${OPENGL_FOUND})
    message("OpenGL Not Found!")
endif()
if(NOT ${FREEGLUT_FOUND})
    message("GLUT Not found!")
endif()
# 包含目录
include_directories(
    ${PROJECT_SOURCE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)
# 显示信息
message("OpenGL: ${OPENGL_INCLUDE_DIR}")
message("Glut: ${GLUT_INCLUDE_DIR}")
# 源代码目录
aux_source_directory(. SRC)
# 添加到可执行文件
add_executable(${PROJECT_NAME} ${SRC})
# 链接目标
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARY} 
    ${GLUT_LIBRARY}
)
