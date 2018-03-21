#ifndef _CG_UTILS_H_
#define _CG_UTILS_H_

#include "cg_types.h"
#include "cg_prim2d.hpp"

namespace cg {

/**
 * 检查并输出OpenGL内部错误
 * @return {GLenum} 错误编码
 */
GLenum errorCheck ();

/**
 * 打开OpenGL 窗口
 * @param renderCallback {void(*)()} 窗口绘制（回调）函数（默认为空）
 * @param title {char*} 窗口标题
 * @param initCallback {void(*)()} 窗口初始化（回调）函数（默认为空）
 * @param reshapeCallback {void(*)()} 窗口大小变化（回调）函数（默认为空）
 * @param usingGL3 {bool} 是否启用OpenGL3模式 (默认为false)
 * @param rect {Recti} 窗口矩形区域，默认位置为(100,100)，默认大小为(800,600)
 */
void openGlWindow ( 
    void(*renderCallback)(),
    const char* title = "Computer Graphics with OpenGL",
    void(*initGlCallback)() = NULL,
    void(*reshapeCallback)(int, int) = NULL,
    bool usingGL3 = false,
    Recti rect = Recti(100, 100, 800, 600)
);

/**
 * 加载着色器
 * @param vertexShaderFilePath {char*} 顶点着色器文件的路径
 * @param fragmentShaderFilePath {char*} 片元着色器文件的路径
 */
GLuint LoadShaders(const char * vertexShaderFilePath, 
    const char * fragmentShaderFilePath);



} //namespace cg

#endif //_CG_UTILS_H_