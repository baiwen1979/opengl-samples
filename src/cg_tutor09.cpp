#include <cg_utils.h>
#include <cgm/cg_math.h>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 窗口设置
static const Recti WinRect = Recti(100, 100, 1024, 768);

// 顶点数组对象VAO的ID
static GLuint vertexArrayID;
// 顶点缓存对象VBO的ID
static GLuint vertexBuffer;
// 索引缓存对象IBO的ID
static GLuint indexBuffer;
// 着色器变量的位置
static GLuint glWorldLocation;
// 透视变换参数设置
static PersProjParams persProjParams;

// 着色器文件路径
static const char* vShaderFileName = "GLSL/vshader07.glsl";
static const char* fshaderFileName = "GLSL/fshader07.glsl";

Mat4f testMatrixTransform() {
    Mat4f trans;
    cout << trans << endl;
    trans = Mat4f::scale(trans, Vec3f(1.2f));
    //cout << trans << endl;
    //trans = Mat4f::rotate(trans, -60.0f, Vec3f(0.0f, 1.0f, 0.0f));
    trans = Mat4f::rotateZ(trans, -60.0f);
    //cout << trans << endl;
    trans = Mat4f::translate(trans, Vec3f(0.0f, 0.0f, 5.0f));
    cout << trans << endl;

    Mat4f projectMat = Mat4f::perspective(45, (float)WinRect.w/(float)WinRect.h, 1.0f, 100.0f);

    cout << projectMat << endl;
    trans =  projectMat * trans;
    cout << trans << endl;
    return trans;
}

 /**
 * 渲染回调函数
 */
static void renderSceneCB() {
    // 旋转角度
    static float scale = 0.5f;
    // 旋转增量
    scale += 0.5f;
 
    Pipeline p;    
    p.rotate(0.0f, scale, 0.0f);
    p.posit(0.0f, 0.0f, 5.0f);   
    // 设置透视投影变换参数
    p.setPersProjParams(persProjParams);
    // 获得世界坐标透视投影变换矩阵
    const Mat4f& trans = p.getWPTransform();

    // 将平移矩阵传递给着色器的uniform变量glWorld中
    glUniformMatrix4fv(glWorldLocation, 1, GL_TRUE, &trans[0][0]);

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // 绑定到事先创建好的顶点数组对象
    glBindVertexArray(vertexArrayID);

    // 绘制图形元素
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // 交换前后缓存
    glutSwapBuffers();
}

/**
 * 创建顶点缓冲器
 */
static void createVertexBuffer()
{
    // 金字塔（4面体）的4个顶点
    Vec3f vertices[4];
    // 金字塔4个顶点位置
    vertices[0] = Vec3f(-1.0f, -1.0f, 0.5773f);
    vertices[1] = Vec3f(0.0f, -1.0f, -1.15475f);
    vertices[2] = Vec3f(1.0f, -1.0f, 0.5773f);
    vertices[3] = Vec3f(0.0f, 1.0f, 0.0f);

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

    // 顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // 开启顶点位置属性
    glEnableVertexAttribArray(0);
}

// 创建索引缓冲器
static void createIndexBuffer() {
    GLuint indices[] = { 
        0, 3, 1, 
        1, 3, 2, 
        2, 3, 0, 
        0, 1, 2
    };

    // 创建缓冲区
    glGenBuffers(1, &indexBuffer);
    // 绑定缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // 添加缓冲区数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

static void initPersProjInfo() {
    persProjParams.aspectRatio = WinRect.w / WinRect.h;
    persProjParams.fieldOfView = 60.0f;
    persProjParams.zNear = 1.0f;
    persProjParams.zFar = 100.0f;
}

static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置前向面
    glFrontFace(GL_CW);
    // 背面剔除
    glCullFace(GL_BACK);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

static void init() {
    initGl();
    createVertexBuffer();
    createIndexBuffer();
    initShaders();
    initPersProjInfo();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 09 - Perspective Projection", 
        init, 
        NULL, 
        renderSceneCB,
        true, 
        WinRect
    );
}
*/





