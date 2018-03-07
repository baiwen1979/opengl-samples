# 最小版本
cmake_minimum_required(VERSION 2.8)
# 项目名称
project(cg_opengl)
# 查找依赖包
find_package(OpenGL REQUIRED)
find_package(GLUT QUIET)
# 库目录
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
# 查找失败
if(NOT ${OPENGL_FOUND})
    message("OpenGL Not Found!")
endif()
# 根据不同目标系统，设置不同的库目录
if(UNIX)
    set(PROJECT_LIB_DIR ${PROJECT_LIB_DIR}/linux)
endif()
if(WIN32)
    set(PROJECT_LIB_DIR ${PROJECT_LIB_DIR}/win32)
endif()
if(APPLE)
    set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
endif()
message("Link Dir: ${PROJECT_LIB_DIR}")
link_directories(${PROJECT_LIB_DIR})
# GLUT库查找失败
if(NOT ${GLUT_FOUND} AND WIN32)
    message("Try To Use Win32 GLUT Library!")
    set(GLUT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/gl)
    set(GlUT_LIBRARY freeglut)
endif()
# 包含目录
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
)
# 显示信息
message("OpenGL Include Dir: ${OPENGL_INCLUDE_DIR}")
message("OpenGL Library: ${OPENGL_LIBRARY}")
message("GLUT Include Dir: ${GLUT_INCLUDE_DIR}")
message("GLUT Library: ${GLUT_LIBRARY}")
# 设置变量
set("PROJECT_LIBS" ${OPENGL_LIBRARY} ${GLUT_LIBRARY})
# 源代码目录
aux_source_directory(. SRC_DIR)
# 添加到可执行文件
add_executable(${PROJECT_NAME} ${SRC_DIR})
# 链接目标
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
