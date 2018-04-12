#ifndef _CG_INIT_HPP_
#define _CG_INIT_HPP_

#include <cg_types.h>

namespace glw {

/**
 * 打开OpenGL 窗口
 * @param renderCallback {void(*)()} 窗口绘制（回调）函数（默认为空）
 * @param title {char*} 窗口标题
 * @param initCallback {void(*)()} 窗口初始化（回调）函数（默认为空）
 * @param reshapeCallback {void(*)(int, int)} 窗口大小变化（回调）函数（默认为空）
 * @param idleCallback {void(*)()} 空闲（动画）回调函数（默认为空）
 * @param usingGL3 {bool} 是否启用OpenGL3模式 (默认为false)
 * @param rect {Recti} 窗口矩形区域，默认位置为(100,100)，默认大小为(800,600)
 */
void openGlWindow ( 
    void(*renderCallback)(),
    const char* title = "Computer Graphics with OpenGL",
    void(*initGlCallback)() = NULL,
    void(*reshapeCallback)(int, int) = NULL,
    void(*idleCallback)() = NULL,
    bool usingGL3 = false,
    const cg::Recti& rect = cg::Recti(100, 100, 800, 600)
);


} //namespace glw

#endif //_CG_INIT_HPP_