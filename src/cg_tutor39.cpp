#include <cg_utils.h>
#include <cgm/cg_math.h>

#include <vector>
#include <map>
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
static Model * pModelPlane = NULL;

// 摄像机
static Camera * pCamera = NULL;
// 着色器
static Shader * pModelShader = NULL; // 模型着色器
// 纹理
static Texture * pFloorTexture = NULL; // 纹理用于地面
static Texture * pFloorTextureGammaCorrected = NULL; //伽马校正的纹理
// 开启blinn光照?
bool blinn = false;
// 启用Gamma修正？
bool gammaEnabled = false;

Vec3f lightPositions[] = {
    Vec3f(-0.6f, 0.1f, 0.0f),
    Vec3f(-0.2f, 0.1f, 0.0f),
    Vec3f( 0.2f, 0.1f, 0.0f),
    Vec3f( 0.6f, 0.1f, 0.0f)
};

Vec3f lightColors[] = {
    Vec3f(0.25),
    Vec3f(0.50),
    Vec3f(0.75),
    Vec3f(1.00)
};

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
        case 'b':
            blinn = !blinn;
            break;
        case 'g':
            gammaEnabled = !gammaEnabled;
            break;
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

// 初始化OpenGL状态参数
static void initGl() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 深度测试函数
    glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 启用面剔除
    glEnable(GL_CULL_FACE);
    // 指定要剔除的面(背面)
    glCullFace(GL_BACK);
    // 设置前向面（逆时针）
    glFrontFace(GL_CCW);

    // 背景颜色
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

// 窗口调整回调函数
static void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

static Vec3f lightPos(0.0f, 0.2f, 0.0f); 

static void renderModel(float deltaTime) {
    Mat4f projection = pCamera -> getPerspectiveMatrix();
    Mat4f view = pCamera -> getViewMatrix();

    pModelShader -> use();

    pModelShader -> setMat4f("projection", projection);
    pModelShader -> setMat4f("view", view);

    // 光照
    pModelShader -> setVec3fv("lightPositions", lightPositions, 4);
    pModelShader -> setVec3fv("lightColors", lightColors, 4);
    pModelShader -> setVec3f("viewPos", pCamera -> getPosition());
    pModelShader -> setBool("gamma", gammaEnabled);

    Mat4f model = Mat4f::scale(Mat4f(), Vec3f(4.0f));
    pModelShader -> setMat4f("model", model);
    if (gammaEnabled) {
        pFloorTexture -> apply(GL_TEXTURE0);
    }
    else {
        pFloorTextureGammaCorrected -> apply(GL_TEXTURE0);
    }
    pModelPlane -> render(pModelShader);

}

/* 渲染回调函数 */
static void renderSceneCB() {
    // 清空颜色、深度和蒙版缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    static float deltaTime = 0.0f;

    renderModel(deltaTime);

    // 交换前后缓存
    glutSwapBuffers();
    deltaTime += 0.1;
}

/* 模型初始化 */
static void initModel() {
    pModelCube = new Model("models/cube.obj");
    pModelPlane= new Model("models/plane.obj");
}

static void initTextures() {
    pFloorTexture = new Texture("textures/wood.png", Texture::Diffuse, false);
    pFloorTextureGammaCorrected = new Texture("textures/wood.png", Texture::Diffuse);
}

/* 初始化着色器 */
static void initShaders() {
    pModelShader = new Shader(
        "GLSL/vs39.gammaCorrection.glsl", 
        "GLSL/fs39.gammaCorrection.glsl"
    );
}

/* 初始化摄像机 */
static void initCamera() {
    pCamera = new Camera((float)WIN_RECT.w / (float)WIN_RECT.h, Vec3f(0.0f, 0.5f, 3.0f));
}

/* 初始化灯光 */
static void initLight() {
}

/* 初始化材质 */
static void initMaterial() {
    // 模型
    pModelShader -> use();
    pModelShader -> setInt("texture1", 0);
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
        "OpenGL Tutorial 39 - Advanced Lighting: Blinn - Phong Lighting and Gamma Correction", 
        init, 
        reshape, 
        renderSceneCB,
        true, 
        WIN_RECT
    );
}
*/