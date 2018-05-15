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
static Model * pModelPlanet;
static Model * pModelRock;
// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pShader = NULL;
// 模型变换矩阵
static Mat4f * pModelMatrices;
// 数量
static unsigned int amount = 10000;

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

static void renderModel(float deltaTime) {
    pShader -> use();
    Mat4f projection = pCamera -> getPerspectiveMatrix();
    Mat4f view = pCamera -> getViewMatrix();;
    pShader -> setMat4f("projection", projection);
    pShader -> setMat4f("view", view);

    // draw planet
    Mat4f model;
    model = Mat4f::scale(model, Vec3f(4.0f));
    model = Mat4f::rotateY(model, deltaTime);
    model = Mat4f::translate(model, Vec3f(0.0f, -2.0f, 0.0f));
    pShader -> setMat4f("model", model);
    pModelPlanet -> render(pShader);

    // draw meteorites
    for (unsigned int i = 0; i < amount; i++)
    {
        pShader -> setMat4f("model", pModelMatrices[i]);
        pModelRock -> render(pShader);
    }     
}

/* 渲染回调函数 */
static void renderSceneCB() {
    static float deltaTime = 0.0f;
    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // 清空颜色和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 渲染模型
    renderModel(deltaTime);
    
    // 交换前后缓存
    glutSwapBuffers();
    deltaTime += 0.1;
}

/* 模型初始化 */
static void initModel() {
    pModelPlanet = new Model("models/planet/planet.obj");
    pModelRock = new Model("models/rock/rock.obj");
}

static void initModelMatrices() {
    pModelMatrices = new Mat4f[amount];
    srand(0.1); // initialize random seed	
    float radius = 50.0f;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        Mat4f model;
        // 1. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = Mat4f::scale(model, Vec3f(scale));
        // 2. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = Mat4f::rotate(model, rotAngle, Vec3f(0.4f, 0.6f, 0.8f));
        // 3. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = Mat4f::translate(model, Vec3f(x, y, z));

        // 4. now add to list of matrices
        pModelMatrices[i] = model;
    }
}

/* 初始化着色器 */
static void initShaders() {
    pShader = new Shader(
        "GLSL/vs37.instancing.glsl", 
        "GLSL/fs37.instancing.glsl"
    );
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.0f, 55.0f));
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
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// 初始化
static void init() {
    initGl();
    
    initShaders();
    initModel();
    initModelMatrices();

    initCamera();
    initLight();
    initMaterial();

    registerUIEvents();
}

/*
void testOGLTutorial() {
    glw::openGlWindow(
        renderSceneCB, 
        "OpenGL Tutorial 37 - Instancing : An asteroid field without instancing", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/