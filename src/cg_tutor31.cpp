#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <sstream>
#include <cg_model.hpp>
#include <cg_skybox.hpp>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

// 鼠标设置
static bool firstMouse = true;
static float lastX = WIN_RECT.w / 2.0f;
static float lastY = WIN_RECT.h / 2.0f;

// 模型
static Model * pModel = NULL;
// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pShaderRed = NULL;
static Shader * pShaderGreen = NULL;
static Shader * pShaderBlue = NULL;
static Shader * pShaderYellow = NULL;
// 统一缓存对象(多个矩阵)
static unsigned int uboMatrices;

// 特殊按键处理函数
static void specialKeyboardCB(int key, int x, int y) {
    switch (key) {    
    case GLUT_KEY_UP:
        pCamera -> processKeyboardInput(Camera::MovementDirection::FORWARD, 0.1f);
        break;
    case GLUT_KEY_DOWN:
        pCamera -> processKeyboardInput(Camera::MovementDirection::BACKWARD, 0.1f);
        break;
    case GLUT_KEY_LEFT:
        pCamera -> processKeyboardInput(Camera::MovementDirection::LEFT, 0.1f);
        break;
    case GLUT_KEY_RIGHT:
        pCamera -> processKeyboardInput(Camera::MovementDirection::RIGHT, 0.1f);
    }
}

// 普通按键处理函数
static void keyboardCB(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
    }
}

// 鼠标事件处理函数
static void passiveMouseCB(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xoffset = x - lastX;
    float yoffset = lastY - y;

    lastX = x;
    lastY = y;

    pCamera -> processMouseMovement(xoffset, yoffset);
}

// 注册UI事件处理函数
static void registerUIEvents() {
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
    // 移动鼠标到指定位置
    glutWarpPointer(lastX, lastY);
}

static void setUniformBlockMatrix(const Mat4f& m, GLintptr offset) {
    Mat4f transposed = m.transposed();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Mat4f), &transposed);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static void renderModel() {

    setUniformBlockMatrix(pCamera -> getPerspectiveMatrix(), 0);
    setUniformBlockMatrix(pCamera -> getViewMatrix(), sizeof(Mat4f));
   
    // 红色
    pShaderRed -> use();
    Mat4f model;
    model = Mat4f::scale(model, Vec3f(0.5));
    model = Mat4f::translate(model, Vec3f(-0.75f, 0.75f, 0.0f));
    pShaderRed -> setMat4f("model", model);
    pModel -> render(pShaderRed);
    // 绿色
    pShaderGreen -> use();
    model = Mat4f::scale(Mat4f(), Vec3f(0.5));
    model = Mat4f::translate(model, Vec3f(0.75f, 0.75f, 0.0f));
    pShaderGreen -> setMat4f("model", model);
    pModel -> render(pShaderGreen);
    // 蓝色
    pShaderBlue -> use();
    model = Mat4f::scale(Mat4f(), Vec3f(0.5));
    model = Mat4f::translate(model, Vec3f(-0.75f, -0.75f, 0.0f));
    pShaderBlue -> setMat4f("model", model);
    pModel -> render(pShaderBlue);
    // 黄色
    pShaderYellow -> use();
    model = Mat4f::scale(Mat4f(), Vec3f(0.5));
    model = Mat4f::translate(model, Vec3f(0.75f, -0.75f, 0.0f));
    pShaderYellow -> setMat4f("model", model);
    pModel -> render(pShaderYellow);
}


/* 渲染回调函数 */
static void renderSceneCB() {
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 渲染模型
    renderModel();
    
    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModel = new Model("models/sphere.obj");
}

static void initTextures() {

}

// 初始化矩阵统一块的统一缓存
static void initUniformBufferOfMatrices() {
    // 首先. 获取着色器中的统一块索引（Block Index)
    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(pShaderRed -> getId(), "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(pShaderGreen  -> getId(), "Matrices");
    unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(pShaderBlue -> getId(), "Matrices");
    unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(pShaderYellow -> getId(), "Matrices");

    // 然后，将每个着色器的统一块（Uniform Block)索引链接到统一绑定点0
    glUniformBlockBinding(pShaderRed -> getId(), uniformBlockIndexRed, 0);
    glUniformBlockBinding(pShaderGreen -> getId(), uniformBlockIndexGreen, 0);
    glUniformBlockBinding(pShaderBlue -> getId(), uniformBlockIndexBlue, 0);
    glUniformBlockBinding(pShaderYellow -> getId(), uniformBlockIndexYellow, 0);

    // 创建统一缓存对象
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Mat4f), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // 定义链接到区绑定点的缓存区域
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(Mat4f));

}

/* 初始化着色器 */
static void initShaders() {
    pShaderRed = new Shader("GLSL/vs31.advanced.glsl", "GLSL/fs31.red.glsl");
    pShaderGreen = new Shader("GLSL/vs31.advanced.glsl", "GLSL/fs31.green.glsl");
    pShaderBlue = new Shader("GLSL/vs31.advanced.glsl", "GLSL/fs31.blue.glsl");
    pShaderYellow = new Shader("GLSL/vs31.advanced.glsl", "GLSL/fs31.yellow.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
}

// 初始化OpenGL参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 设置前向面
    glFrontFace(GL_CCW);
    // 背面剔除
    glCullFace(GL_BACK);
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// 初始化
static void init() {
    initGl();
    
    initShaders();
    initTextures();
    initModel();

    initUniformBufferOfMatrices();

    initCamera();
    initLight();
    initMaterial();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 31 - Advanced GLSL: Uniform Buffer Object", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/