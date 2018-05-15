#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <sstream>
#include <cg_model.hpp>
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
static Model * pModelCube = NULL;
static Model * pModelFloor = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pShader = NULL;
// 纹理
static Texture * pCubeTexture = NULL;  // 纹理用于立方体
static Texture * pFloorTexture = NULL; // 纹理用于平面

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.1f; 

    pShader -> use();
    pShader -> setMat4f("view", pCamera -> getViewMatrix());
    pShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    
    // 渲染立方体
    Mat4f model;
    model = Mat4f::rotateY(model, angle);
    model = Mat4f::translate(model, Vec3f(-1.0f, 0.0f, -3.0f));
    pShader -> setMat4f("model", model);
    pCubeTexture -> apply(GL_TEXTURE0);
    pModelCube -> render(*pShader);

    model = Mat4f::rotateY(Mat4f(), angle);
    model = Mat4f::translate(model, Vec3f(1.0f, 0.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pModelCube -> render(*pShader);

    // 渲染地面
    model = Mat4f();
    model = Mat4f::scale(model, Vec3f(8.0f));
    model = Mat4f::translate(model, Vec3f(0.0f, -1.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pFloorTexture -> apply(GL_TEXTURE0);
    pModelFloor -> render(*pShader); 
    
    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelFloor= new Model("models/plane.obj");
}

static void initTextures() {
    pCubeTexture = new Texture("textures/container2.png", Texture::Diffuse);
    pFloorTexture = new Texture("textures/test.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pShader = new Shader("GLSL/vs26.depthTest.glsl", "GLSL/fs26.depthTest.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 5.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
    pShader -> use();
    pShader -> setInt("texture1", 0);
}

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

// 初始化OpenGL参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);
    // 设置前向面
    glFrontFace(GL_CCW);
    // 背面剔除
    glCullFace(GL_BACK);
    // 启用背面剔除
    glEnable(GL_CULL_FACE);
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
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

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 26 - Depth Tesing", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/