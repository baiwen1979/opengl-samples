## 最小版本
cmake_minimum_required(VERSION 2.8)
## 项目名称
project(CGLab)

## 设置项目构建的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

## 查找构建项目必需的系统OpenGL依赖包
find_package(OpenGL REQUIRED)
# 查找OpenGL失败
if(NOT ${OPENGL_FOUND})
    message("OpenGL Not Found!")
endif()
# 查找成功
# 显示OpenGL的系统包含目录
message("OpenGL Include Directory: ${OPENGL_INCLUDE_DIR}")
message("OpenGL Library Directory: ${OPENGL_LIBRARY}")

## 项目包含目录
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
# 设置编译器包含目录
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${PROJECT_INCLUDE_DIR}
)

## 查找并设置项目所需的第三方依赖包
# 查找GLUT依赖包
find_package(GLUT QUIET)
# 查找GLUT失败
if(NOT ${GLUT_FOUND})
    # 使用项目GLUT库
    set(GLUT_INCLUDE_DIR ${PROJECT_INCLUDE_DIR})
    set(GLUT_LIBRARY freeglut)
endif()
# 显示GLUT包含目录
message("GLUT Include Directory: ${GLUT_INCLUDE_DIR}")
# 显示GLUT库
message("GLUT Library: ${GLUT_LIBRARY}")
# 查找GLEW依赖包
find_package(GLEW QUIET)
# 查找GLEW失败
if(NOT ${GLEW_FOUND})
    # 使用项目GLEW库
    set(GLEW_INCLUDE_DIR ${PROJECT_INCLUDE_DIR})
    set(GLEW_LIBRARY GLEW)
endif()
# 显示GLEW包含目录
message("GLEW Include Directory: ${GLEW_INCLUDE_DIR}")
# 显示GLEW库
message("GLEW Library: ${GLEW_LIBRARY}")

#### 第三方库目录
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
# 指定LINUX系统的第三方库目录
if(UNIX AND NOT APPLE)
    set(PROJECT_LIB_DIR ${PROJECT_LIB_DIR}/linux)
endif()
# 指定Win32系统的第三方库目录
if(WIN32)
    set(PROJECT_LIB_DIR ${PROJECT_LIB_DIR}/win32)
endif()
# 制定APPLE系统的第三方库目录
if(APPLE)
    set(PROJECT_LIB_DIR ${PROJECT_LIB_DIR}/macos)
endif()
# 显示第三方库目录
message("Project Library Directory: ${PROJECT_LIB_DIR}")
# 将第三方库目录设置为链接目录
link_directories(${PROJECT_LIB_DIR})

# 添加项目工具库子目录
add_subdirectory(utils)
# 添加项目3D几何对象库子目录
add_subdirectory(math3d)
# 设置项目自定义库
set(PROJECT_LIBS CGUtils CGMath3d)

# 将SRC目录下的所有*.cpp和*.hpp文件作为编译器输入
file(GLOB SRC_FILES "./src/*.hpp" "./src/*.cpp")
# 添加到可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 设置链接库变量
set(LINK_LIBS 
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    ${GLUT_LIBRARY}
    ${PROJECT_LIBS}
)
# 指定目标链接库
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
