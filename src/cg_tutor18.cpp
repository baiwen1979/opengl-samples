#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <cg_model.hpp>
#include <cassert>

#include "cg_glw.hpp"

using namespace cg;

// 窗口设置
static const Recti WIN_RECT = Recti(100, 100, 1024, 768);

static bool firstMouse = true;
static float lastX = WIN_RECT.w / 2.0f;
static float lastY = WIN_RECT.h / 2.0f;

// 网格对象
static Model* pModel = NULL;
// 摄像机
static Camera *pCamera = NULL;
// 着色器
static Shader* pLightingShader = NULL;
static Shader* pLampShader = NULL;

 /* 渲染回调函数 */
static void renderSceneCB() {

    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 旋转角度
    static float angle = 0.0f;
    // 旋转增量
    angle += 0.5f;
    pLightingShader -> use();
    pLightingShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pLightingShader -> setMat4f("view", pCamera -> getViewMatrix());
    // 模型矩阵
    Mat4f model = Mat4f::scale(Mat4f(), Vec3f(1.0f));
    model = Mat4f::rotateY(model, angle);
    model = Mat4f::rotateX(model, angle);
    model = Mat4f::translate(model, Vec3f(0.0f, 0.0f, -2.0f));
    // 模型相对于摄像机移动
    model = Mat4f::translate(model, pCamera -> getPosition() * -1);
    pLightingShader -> setMat4f("model", model);
    
    // 绘制立方体模型
    pModel->render(*pLightingShader);

    // 灯
    pLampShader -> use();
    pLampShader -> setMat4f("projection", pCamera -> getPerspectiveMatrix());
    pLampShader -> setMat4f("view", pCamera -> getViewMatrix());
    model = Mat4f::scale(Mat4f(), Vec3f(0.15f));
    model = Mat4f::translate(model, Vec3f(-3.0f, 1.0f, -1.0f));   
    model = Mat4f::translate(model, pCamera -> getPosition() * -1);
    pLampShader -> setMat4f("model", model);
    pModel->render(*pLampShader);

    // 交换前后缓存
    glutSwapBuffers();
}

/* 模型初始化 */
static void initModel() {
    pModel = new Model("models/cube.obj");
}

/* 初始化着色器 */
static void initShaders() {
    pLightingShader = new Shader("GLSL/vs18.colors.glsl", "GLSL/fs18.colors.glsl");
    pLampShader = new Shader("GLSL/vs18.lamp.glsl", "GLSL/fs18.lamp.glsl");
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
    pLightingShader -> use();
    pLightingShader -> setColor3f("objectColor", Color3f(1.0f, 0.5f, 0.31f));
    pLightingShader -> setColor3f("lightColor", Color3f(1.0f, 1.0f, 1.0f));
}

/* 初始化材质 */
static void initMaterial() {
    // nothing to do here
}


// 事件处理函数
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

static void keyboardCB(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
    }
}

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

static void registerUIEvents() {
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
    // 移动鼠标到指定位置
    glutWarpPointer(lastX, lastY);
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

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

static void init() {
    initGl();
    initModel();
    initShaders();
    initCamera();
    initLight();
    initMaterial();
    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 18 - Lighting: Colors", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/
