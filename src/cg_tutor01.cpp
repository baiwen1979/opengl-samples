#include <cg_utils.h>
#include <cgm/cg_math.h>

#include "cg_glw.hpp"

using namespace cg;
using namespace glw;

static GLuint vbo;

 /**
 * 渲染回调函数
 */
static void renderSceneCB(){
    // 清空颜色缓存
    glClear(GL_COLOR_BUFFER_BIT);

    // 开启顶点属性
    glEnableVertexAttribArray(0);
    // 绑定GL_ARRAY_BUFFER缓冲器
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 告诉管线怎样解析bufer中的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // 开始绘制几何图形(绘制一个点)
    glDrawArrays(GL_POINTS, 0, 1);

    //  禁用顶点数据
    glDisableVertexAttribArray(0);

    // 交换前后缓存
    glutSwapBuffers();
}

/**
 * 创建顶点缓冲器
 */
static void createVertexBuffer()
{
    // 创建含有一个顶点的顶点数组
    Vec3f vertices[1];
    // 将点置于屏幕中央
    vertices[0] = Vec3f(0.0f, 0.0f, 0.0f);
    // 创建缓冲器
    glGenBuffers(1, &vbo);
    // 绑定GL_ARRAY_BUFFER缓冲器
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 绑定顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void init() {
    createVertexBuffer();
}

/*
void testOGLTutorial() {
    openGlWindow(renderSceneCB, "OpenGL Tutorials - Point", init);
}
*/

 