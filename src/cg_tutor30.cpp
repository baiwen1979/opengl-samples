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

// 天空盒
static Skybox * pSkybox = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pShader = NULL;
static Shader * pSkyboxShader = NULL;
// 纹理
static Cubemap * pCubemap = NULL;  // 立方体贴图

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
    float xoffset = lastX - x;
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

static void renderModel() {
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f; 

    pShader -> use();
    pShader -> setMat4f("view", pCamera -> getViewMatrix());
    pShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pShader -> setVec3f("cameraPos", pCamera -> getPosition());
    
    // 渲染模型
    Mat4f model;
    model = Mat4f::rotateY(model, angle);
    model = Mat4f::translate(model, Vec3f(0.0f, 0.0f, -3.0f));
    pShader -> setMat4f("model", model);
    pCubemap -> apply(GL_TEXTURE0);
    pModel -> render(pShader);
}

static void renderSkybox() {
    pSkyboxShader -> use();
    Mat4f view = Mat4f(Mat3f(pCamera -> getViewMatrix()));
    pSkyboxShader -> setMat4f("view", view);
    pSkyboxShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pSkybox -> render(pSkyboxShader);
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
    // 渲染天空
    renderSkybox();
    
    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    //pModel = new Model("models/nanosuit/nanosuit.obj");
    pModel = new Model("models/sphere.obj");
    pSkybox = new Skybox(*pCubemap);
}

static void initTextures() {
    pCubemap = new Cubemap(
        "textures/skybox/right.jpg", "textures/skybox/left.jpg",
        "textures/skybox/top.jpg", "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg", "textures/skybox/back.jpg"
    );
}

/* 初始化着色器 */
static void initShaders() {
    pShader = new Shader("GLSL/vs30.cubemap.glsl", "GLSL/fs30.cubemap.glsl");
    pSkyboxShader = new Shader("GLSL/vs30.skybox.glsl", "GLSL/fs30.skybox.glsl");
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
    pShader -> use();
    pShader -> setInt("texture1", 0);
    pSkyboxShader -> use();
    pSkyboxShader -> setInt("skybox", 0);
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
    glFrontFace(GL_CW);
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

    initCamera();
    initLight();
    initMaterial();

    registerUIEvents();
}


void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 30 - Cubemap: Skybox, Reflection, Refraction", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
