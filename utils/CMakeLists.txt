# 为当前目录下所有的源文件定义变量
file(GLOB UTILS_LIB_SRC "./*.cpp")
# 创建新库：使用指定的源文件在项目中添加新库
add_library(Utils ${UTILS_LIB_SRC})
# 确保编译器可以找到新库中的文件
target_include_directories(Utils PUBLIC ${PROJECT_INCLUDE_DIR})