#include <cg_utils.h>
#include <cgm/cg_math.h>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 平行光
struct DirectionalLight {
    Color3f color;    // 光的颜色
    float intensity;  // 光的强度
};

// 带纹理坐标的顶点
struct MyVertex {
    Vec3f pos;      // 顶点位置
    Vec2f texCoord; // 纹理坐标
    MyVertex(Vec3f p, Vec2f tc): pos(p), texCoord(tc) {}
};

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

// 顶点数组对象VAO的ID
static GLuint vertexArray;
// 顶点缓存对象VBO的ID
static GLuint vertexBuffer;
// 索引缓存对象IBO的ID
static GLuint indexBuffer;
// 纹理对象
static GLuint texture;

// 着色器中世界观察投影坐标uniform的位置
static GLuint gWVPLocation;
// 着色器中纹理采样uniform的位置
static GLuint gSamplerLocation;
// 着色器中平行光颜色uniform的位置
static GLuint gDirLightColorLocation;
// 着色器中平行光强度uniform的位置
static GLuint gDirLightIntensityLocation;

// 透视变换参数设置
static PersProjParams persProjParams;
// 摄像机
static CameraQuat *pCamera = NULL;

// 着色器文件路径
static const char* vShaderFileName = "GLSL/vshader14.glsl";
static const char* fshaderFileName = "GLSL/fshader14.glsl";

 /**
 * 渲染回调函数
 */
static void renderSceneCB() {
    pCamera -> onRender();
    // 旋转角度
    static float scale = 0.0f;
    // 旋转增量
    scale += 0.5f;
 
    Pipeline p;
    p.posit(0.0f, 0.0f, -3.0f);
    p.rotate(0.0f, scale, 0.0f);
    // 设置摄像机
    p.setCamera(*pCamera);
    // 设置透视投影变换参数
    p.setPersProjParams(persProjParams);
    // 获得世界视图坐标透视投影变换矩阵
    const Mat4f& world = p.getWVPTransform();

    // 将平移矩阵传递给着色器的uniform变量glWorld中
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &world[0][0]);

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 绑定到事先创建好的顶点数组对象
    glBindVertexArray(vertexArray);
 
    // 绘制图形元素
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    // 禁用顶点数据
    // glDisableVertexAttribArray(0);
    // 交换前后缓存
    glutSwapBuffers();
}

/**
 * 创建顶点缓冲器
 */
static void createVertexBuffer()
{
    // 金字塔（4面体）的4个顶点
    MyVertex vertices[] = {
        MyVertex(Vec3f(-1.0f, -1.0f, 0.5773f), Vec2f(0.0f, 0.0f)),
        MyVertex(Vec3f(0.0f, -1.0f, -1.15475f), Vec2f(0.5f, 0.0f)),
        MyVertex(Vec3f(1.0f, -1.0f, 0.5773f), Vec2f(1.0f, 0.0f)),
        MyVertex(Vec3f(0.0f, 1.0f, 0.0f), Vec2f(0.5f, 1.0f))
    };

    // 生成1个顶点数组对象，并将其ID存储到vertexArray变量中
    glGenVertexArrays(1, &vertexArray);
    // 创建1个顶点缓冲对象，并将其ID保存到vertexBuffer中
    glGenBuffers(1, &vertexBuffer);
    // 先绑定顶点数组对象
    glBindVertexArray(vertexArray);

    // 绑定顶点缓冲对象GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // 拷贝顶点数组到顶点缓存对象
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)0);
    // 开启顶点位置属性
    glEnableVertexAttribArray(0);
    // 顶点纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(3 * sizeof(float)));
    // 开启纹理坐标属性
    glEnableVertexAttribArray(1);

    // 暂时先解除绑定，绘制前重新绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 创建索引缓冲器
static void createIndexBuffer() {
    GLuint indices[] = { 
        0, 3, 1, 
        1, 3, 2, 
        2, 3, 0, 
        0, 1, 2
    };

    // 创建索引缓冲对象
    glGenBuffers(1, &indexBuffer);
    // 绑定到顶点数组对象
    glBindVertexArray(vertexArray);
    // 绑定索引缓对象到GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // 拷贝索引数组到索引缓冲
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

// 创建纹理对象
static void createTexture() {
        // 加载纹理图像
    Texture tex = Texture::load("textures/test.png", Texture::Diffuse);
    texture = tex.getId();
}

static void initShaders() {
    // 加载着色器
    GLuint program = LoadShaders(vShaderFileName, fshaderFileName);
    // 使用着色器
    glUseProgram(program);

    // 获取着色器中uniform变量的位置
    gWVPLocation = glGetUniformLocation(program, "gWVP");
    gSamplerLocation = glGetUniformLocation(program, "gSampler");
    gDirLightColorLocation = glGetUniformLocation(program, "gDirectionalLight.Color");
    gDirLightIntensityLocation = glGetUniformLocation(program, "gDirectionalLight.AmbientIntensity");

    // 检查错误
    assert(gWVPLocation != 0xFFFFFFFF 
        && gSamplerLocation != 0xFFFFFFFF 
        && gDirLightColorLocation != 0xFFFFFFFF
        && gDirLightIntensityLocation != 0xFFFFFFFF);
}

static void initPersProjParams() {
    persProjParams.aspectRatio = WIN_RECT.w / WIN_RECT.h;
    persProjParams.fieldOfView = 60.0f;
    persProjParams.zNear = 1.0f;
    persProjParams.zFar = 100.0f;
}

static void initCamera() {
    pCamera = new CameraQuat(WIN_RECT.w, WIN_RECT.h);
}

static void initDirectionalLight() {
    DirectionalLight dirLight;
    dirLight.color = Color3f(0.0, 1.0, 1.0);
    dirLight.intensity = 0.5f;
    glUniform3f(gDirLightColorLocation, dirLight.color.r, dirLight.color.g, dirLight.color.b);
    glUniform1f(gDirLightIntensityLocation, dirLight.intensity);
}


// 事件处理函数
static void specialKeyboardCB(int key, int x, int y) {
    pCamera -> onKeyboard(key);
}

static void keyboardCB(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
    }
}

static void passiveMouseCB(int x, int y) {
    pCamera -> onMouse(x, y);
}

static void registerUIEvents() {
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
}

static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 设置前向面
    glFrontFace(GL_CCW);
    // 背面剔除
    glCullFace(GL_BACK);

    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

static void init() {
    initGl();
    createVertexBuffer();
    createIndexBuffer();
    createTexture();

    initShaders();
    initPersProjParams();
    initCamera();
    initDirectionalLight();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 14 - Ambient Lighting ", 
        init, 
        NULL, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/