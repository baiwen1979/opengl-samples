#include <cg_utils.h>
#include <cgm/cg_math.h>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 顶点数组对象VAO的ID
static GLuint vertexArrayID;
// 顶点缓存对象VBO的ID
static GLuint vertexBuffer;
// 着色器变量的位置
static GLuint glWorldLocation;
// 着色器文件路径
static const char* vShaderFileName = "GLSL/vshader04.glsl";
static const char* fshaderFileName = "GLSL/fshader04.glsl";

 /**
 * 渲染回调函数
 */
static void renderSceneCB() {
    // 平移距离
    static float tx = 0.0f;
    // 平移增量
    tx += 0.01f;
    // 创建4X4平移矩阵
    Mat4f world(
        1.0f, 0.0f, 0.0f, sinf(tx),
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // 将平移矩阵传递给着色器的uniform变量glWorld中
    glUniformMatrix4fv(glWorldLocation, 1, GL_TRUE, &world[0][0]);

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 开启顶点属性
    glEnableVertexAttribArray(0);
    // 绑定GL_ARRAY_BUFFER缓冲器
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // 告诉管线怎样解析buffer中的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // 开始绘制几何图形(绘制一个三角形，从顶点0开始，绘制三个顶点)
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    // 创建含有3个顶点的顶点数组
    Vec3f vertices[3];
    // 三角形的三个顶点位置
    vertices[0] = Vec3f(-0.8f, -0.8f, 0.0f);
    vertices[1] = Vec3f(0.8f, -0.8f, 0.0f);
    vertices[2] = Vec3f(0.0f, 0.8f, 0.0f);
    // 生成1个顶点数组，并将其ID存储到vertexArrayID变量中
    glGenVertexArrays(1, &vertexArrayID);
    // 绑定顶点数组
    glBindVertexArray(vertexArrayID);
    // 创建1个缓冲器，并将其ID保存到vertexBuffer中
    glGenBuffers(1, &vertexBuffer);
    // 绑定GL_ARRAY_BUFFER到缓冲器
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // 绑定顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void initShaders() {
    // 加载着色器
    GLuint program = LoadShaders(vShaderFileName, fshaderFileName);
    // 使用着色器
    glUseProgram(program);
    // 获取着色器中uniform变量glWorld的位置
    glWorldLocation = glGetUniformLocation(program, "glWorld");
    // 检查错误
    assert(glWorldLocation != 0xFFFFFFFF);
}

static void init() {
    cout << "Initialing Vertex Buffer" << endl;
    createVertexBuffer();
    initShaders();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(renderSceneCB, 
        "OpenGL Tutorial 04 - Translation Transformation", 
        init, NULL, renderSceneCB, true, Recti(100,100,1024, 768));
}
*/
