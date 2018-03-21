#include <cg_utils.h>
#include <cg_math3d.h>

using namespace cg;

// 顶点数组对象VAO的ID
static GLuint vertexArrayID;
// 顶点缓存对象VBO的ID
static GLuint vertexBuffer;

 /**
 * 渲染回调函数
 */
static void renderSceneCB() {
    cout << "Rendering" << endl;
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

    glFlush();
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
    // 加载着色器
    GLuint program = LoadShaders(
        "GLSL/SimpleVertexShader.glsl", 
        "GLSL/SimpleFragmentShader.glsl");
    // 使用着色器
    glUseProgram(program);
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

static void init() {
    cout << "Initialing Vertex Buffer" << endl;
    createVertexBuffer();
}


void testOGLTutorial() {
    openGlWindow(renderSceneCB, 
        "OpenGL Tutorial 02 - First Triangle", 
        init, NULL, true, Recti(100,100,1024, 768));
}


 