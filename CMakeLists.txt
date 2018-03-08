# 最小版本
cmake_minimum_required(VERSION 2.8)
# 项目名称
project(cg_lab)
# 构建输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
# 查找依赖包
find_package(OpenGL REQUIRED)
find_package(GLUT QUIET)
# 项目包含路径
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
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

# 设置MSVC所用的GLUT库
if(NOT ${GLUT_FOUND} AND MSVC)
    set(GLUT_INCLUDE_DIR ${PROJECT_INCLUDE_DIR})
    set(GLUT_LIBRARY freeglut)
endif()
# 包含目录
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${PROJECT_INCLUDE_DIR}
)
# 链接目录
link_directories(${PROJECT_LIB_DIR})
# 显示信息
message("OpenGL Include Dir: ${OPENGL_INCLUDE_DIR}")
message("OpenGL Library: ${OPENGL_LIBRARY}")
message("GLUT Include Dir: ${GLUT_INCLUDE_DIR}")
message("GLUT Library: ${GLUT_LIBRARY}")
# 添加工具库子目录
add_subdirectory(utils)
# 源代码目录
aux_source_directory(./src SRC_DIR)
# 项目库
set(PROJECT_LIBS Utils)
# 设置链接库变量
set(LINK_LIBS 
    ${OPENGL_LIBRARY} 
    ${GLUT_LIBRARY} 
    ${PROJECT_LIBS}
)
# 添加到可执行文件
add_executable(${PROJECT_NAME} ${SRC_DIR})
# 链接目标
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
